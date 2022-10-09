# Project 1 - npshell

## Input - Line structure

Each single-line input can be derived from `line`:

```
line     ::=  group "\n"
group    ::=  cmd ( sp ord_pipe )* [ sp num_pipe [ group ] | sp fredir ]

ord_pipe ::=  "|" sp cmd
num_pipe ::=  ( "|" | "!" ) num
fredir   ::=  ">" sp file

cmd      ::=  file ( sp arg )*
arg      ::=  word
file     ::=  word

sp       ::=  " "+
num      ::=  ( "0"..."9" )+
word     ::=  ( num | "a"..."z" | "A"..."Z" | "." | "-" | ":" | "\\ " )+
```
