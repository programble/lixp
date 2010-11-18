import io/[Reader, StringReader]
import structs/ArrayList

import runtime/[LispValue, LispNumber, LispCharacter, LispString, LispSymbol, LispKeyword, LispList]

// Some extra Reader methods we need
extend Reader {
    readWhile: func (chars: String) -> String {
        // Based off readUntil code
        sb := Buffer new(1024)
        while (hasNext?()) {
            c := read()
            if (!chars contains?(c) || (!hasNext?() && c == 8)) {
                break
            }
            sb append(c)
        }
        return sb toString()
    }

    skipWhile: func ~str (chars: String) {
        // Based off skipWhile code
        while (hasNext?()) {
            c := read()
            if (!chars contains?(c)) {
                rewind(1)
                break
            }
        }
    }

    readUntil: func ~str (chars: String) -> String {
        sb := Buffer new (1024)
        while (hasNext?()) {
            c := read()
            if (chars contains?(c) || (!hasNext?() && c == 8)) {
                break
            }
            sb append(c)
        }
        return sb toString()
    }
}

LispReader: class {
    reader: Reader

    init: func (=reader)
    
    init: func ~str (str: String) {
        init(StringReader new(str))
    }

    hasNext?: func -> Bool {
        reader hasNext?()
    }

    skipWhitespace: func {
        reader skipWhile(" \t\r\n")
    }

    read: func -> LispValue {
        // Skip leading whitespace
        skipWhitespace()
        
        dispatch := reader peek()
        
        if (dispatch == ')') {
            /* TODO: Should there be a ReaderException or something? */
            raise(This, "Mismatched parentheses")
        } else if (dispatch == '(') {
            return readList()
        } else if ((dispatch >= '0' && dispatch <= '9') || dispatch == '-') {
            return readNumber()
        } else if (dispatch == '\\') {
            return readCharacter()
        } else if (dispatch == '"') {
            return readString()
        } else if (dispatch == ':') {
            return readKeyword()
        } else if (dispatch == '\'') {
            return readQuote()
        } else if (dispatch == ';') {
            reader skipLine()
            return read()
        } else {
            return readSymbol()
        }
    }

    readAll: func -> ArrayList<LispValue> {
        all := ArrayList<LispValue> new()
        while (hasNext?()) {
            all add(read())
        }
        return all
    }

    readList: func -> LispList {
        // Skip opening paren
        reader read()

        skipWhitespace()
        if (!hasNext?()) {
            raise(This, "Unexpected EOF")
        }

        // Empty list (nil)
        if (reader peek() == ')') {
            reader read()
            return LispList new()
        }

        // Read the first item
        first := read()

        skipWhitespace()

        // Check for improper list
        if (reader peek() == '.') {
            reader read() // Skip .
            second := read()
            reader read() // Skip )
            return LispList new(first, second)
        }

        // Proper list
        list := ArrayList<LispValue> new()
        list add(first)
        while (true) {
            skipWhitespace()
            if (reader peek() == ')') {
                break
            }
            list add(read())
        }
        return LispList new(list)
    }

    readNumber: func -> LispNumber {
        str := reader readUntil(" \t\r\n)")
        if (str contains?('.')) {
            // Due to String toFloat() being very stupid, here is a
            // convoluted conversion using sscanf that really detracts
            // from the beauty of ooc
            f: Float
            valid: Bool = sscanf(str, "%f", f&)
            if (valid) {
                return LispNumber new(f)
            } else {
                raise(This, "Invalid float literal")
            }
        } else {
            // More of that awesome C-esque sscanf stuff!
            i: Int
            valid: Bool = sscanf(str, "%i", i&)
            if (valid) {
                return LispNumber new(i)
            } else {
                raise(This, "Invalid int literal")
            }
        }
    }

    readCharacter: func -> LispCharacter {
        // Skip over \
        reader read()
        if (!reader hasNext?()) {
            raise(This, "Unexpected EOF while reading character literal")
        }
        return LispCharacter new(reader read())
    }

    readString: func -> LispString {
        // Skip over opening "
        reader read()
        if (!reader hasNext?()) {
            raise(This, "Unexpected EOF while reading string literal")
        }
        str := reader readUntil('"')
        // Verify we actually had a closing "
        // If we didn't, it means EOF was hit first, right?
        reader rewind(1)
        if (reader read() != '"') {
            raise(This, "Unexpected EOF while reading string literal")
        }
        return LispString new(str)
    }

    readKeyword: func -> LispKeyword {
        // Skip :
        reader read()
        str := reader readUntil(" \t\r\n)")
        return LispKeyword new(str)
    }

    readQuote: func -> LispList {
        // Skip '
        reader read()

        list := ArrayList<LispValue> new()
        list add(LispSymbol new("quote"))
        list add(read())
        return LispList new(list)
    }

    readSymbol: func -> LispSymbol {
        str := reader readUntil(" \t\r\n)")
        return LispSymbol new(str)
    }
}
