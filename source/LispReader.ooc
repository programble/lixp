import io/[Reader, StringReader]
import structs/ArrayList

import runtime/[LispValue, LispNumber, LispCharacter, LispString, LispSymbol, LispKeyword, LispList]

LispReader: class {
    reader: Reader

    init: func (=reader)
    
    init: func ~str (str: String) {
        init(StringReader new(str))
    }

    hasNext?: func -> Bool {
        reader hasNext?()
    }

    read: func -> LispValue {
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
            all add(read)
        }
        return all
    }
}
