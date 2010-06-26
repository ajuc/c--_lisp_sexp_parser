#!/bin/sh
./lispparser '(list "ala" "ma" () :1 :aaa  :1.2.3 ala.ma.kota 2 (a 1.2 ("ala , ma kota" 3)))'
./lispparser '(list, "ala",,, "ma", (),, :1, :aaa  :1.2.3 ,,,,, ala.ma.kota , 2 (a 1.2 ("ala , ma kota" 3)))'
./lispparser '(()()())(()()())'
./lispparser '((((()())))(((())))())'
./lispparser '()'
./lispparser '( )'
./lispparser '[() ]'
./lispparser ' (
{ 1 2 3 [ala ma kota] {1 2
[] () {}
{ [ ( ( ( 1 ) 1
 )
 )

123.2 ( )

 ] }
} }
 ) '

