import io/[Reader, StringReader]
import structs/ArrayList
import text/EscapeSequence

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
                rewind(1)
                break
            }
            sb append(c)
        }
        return sb toString()
    }
}

SyntaxException: class extends Exception {
    init: func (=message)
}

EOFException: class extends SyntaxException {
    init: func (what: Class) {
        message = "Unexpected EOF while reading a %s" format(what name)
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
        
        if (dispatch >= '0' && dispatch <= '9') {
            readNumber()
        } else if (dispatch == '-' && hasNext?()) {
            reader read() // Skip -
            next := reader read()
            reader rewind(2)
            if (next >= '0' && next <= '9') {
                readNumber()
            } else {
                readSymbol()
            }
        } else match (dispatch) {
            case ')' => SyntaxException new("Mismatched parentheses") throw()
            case '(' => readList()
            case '\\' => readCharacter()
            case '"' => readString()
            case ':' => readKeyword()
            case '\'' => readQuote()
            case ';' =>
                reader skipLine()
                null
            case => readSymbol()
        }
    }

    readAll: func -> ArrayList<LispValue> {
        all := ArrayList<LispValue> new()
        while (hasNext?()) {
            x := read()
            if (x != null) {
                all add(x)
            }
        }
        return all
    }

    readList: func -> LispList {
        // Skip opening paren
        reader read()

        skipWhitespace()
        if (!hasNext?()) {
            EOFException new(LispList) throw()
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
            skipWhitespace()
            // EOF at end of list with no )
            if (!reader hasNext?()) {
                EOFException new(LispImproperList) throw()
            }
            if (reader peek() != ')') {
                SyntaxException new("Invalid improper list") throw()
            }
            // Skip )
            reader read()
            return LispList new(first, second)
        }

        // Proper list
        list := ArrayList<LispValue> new()
        list add(first)
        while (true) {
            skipWhitespace()
            // EOF in middle of list
            if (!reader hasNext?()) {
                EOFException new(LispProperList) throw()
            }
            if (reader peek() == ')') {
                reader read()
                break
            }
            list add(read())
        }
        return LispList new(list)
    }

    readNumber: func -> LispNumber {
        str := reader readUntil(" \t\r\n)")
        if (str contains?('.')) {
            // Using sscanf because toFloat() doesn't tell me if it's invalid
            // Little hack to tell if there's trailing garbage
            f: Float
            c: Char
            valid: Int = sscanf(str, "%f%c", f&, c&)
            if (valid == 1) {
                return LispNumber new(f)
            } else {
                SyntaxException new("Invalid float literal") throw()
            }
        } else {
            // The same hack again to tell if there's trailing garbage
            i: Int
            c: Char
            valid: Int = sscanf(str, "%i%c", i&, c&)
            if (valid == 1) {
                return LispNumber new(i)
            } else {
                SyntaxException new("Invalid number literal") throw()
            }
        }
    }

    readCharacter: func -> LispCharacter {
        // Skip over \
        reader read()
        reader read() // No idea why this second read is required :\  
        if (!reader hasNext?()) {
            EOFException new(LispCharacter) throw()
        }
        return LispCharacter new(reader read())
    }

    readString: func -> LispString {
        // Skip over opening "
        reader read()
        if (!reader hasNext?()) {
            EOFException new(LispString) throw()
        }
        str := reader readUntil('"')
        // Verify we actually had a closing "
        // If we didn't, it means EOF was hit first, right?
        reader rewind(1)
        if (reader read() != '"') {
            EOFException new(LispString) throw()
        }
        return LispString new(EscapeSequence unescape(str))
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
