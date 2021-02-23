# anltr4

## 参考资料

https://www.antlr.org/
https://github.com/antlr/antlr4
https://wizardforcel.gitbooks.io/antlr4-short-course/content/getting-started.html

https://developer.ibm.com/zh/articles/j-lo-antlr/
https://www.cnblogs.com/clonen/p/9083359.html

## install

```shell
curl -O https://www.antlr.org/download/antlr-4.9.1-complete.jar
export CLASSPATH=".:${PWD}/antlr-4.9.1-complete.jar:$CLASSPATH"
alias antlr4='java -jar ${PWD}/antlr-4.9.1-complete.jar'
alias grun='java org.antlr.v4.gui.TestRig'
```

## example

```g4
grammar Math;

// @header{package zjh;} 

prog : stat+;

stat: expr NEWLINE          # printExpr
    | ID '=' expr NEWLINE   # assign
    | NEWLINE               # blank
    ;

expr:  expr op=('*'|'/') expr   # MulDiv
| expr op=('+'|'-') expr        # AddSub
| INT                           # int
| ID                            # id
| '(' expr ')'                  # parens
;

MUL : '*' ; // assigns token name to '*' used above in grammar
DIV : '/' ;
ADD : '+' ;
SUB : '-' ;
ID : [a-zA-Z]+ ;
INT : [0-9]+ ;
NEWLINE:'\r'? '\n' ;
WS : [ \t]+ -> skip;
```

## 编译g4

文件名必须和g4文件中的grammar名字一致。
```shell
antlr4 MathExpression.g4
```

生成java文件
* MathLexer.java
* MathListener.java: interface
* MathBaseListener.java: 空的base实现
* MathParser.java

```java
// import ANTLR's runtime libraries
import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.tree.*;

public class Test {
    public static void main(String[] args) throws Exception {
        // create a CharStream that reads from standard input
        ANTLRInputStream input = new ANTLRInputStream(System.in);

        // create a lexer that feeds off of input CharStream
        MathLexer lexer = new MathLexer(input);

        // create a buffer of tokens pulled from the lexer
        CommonTokenStream tokens = new CommonTokenStream(lexer);

        // create a parser that feeds off the tokens buffer
        MathParser parser = new MathParser(tokens);

        ParseTree tree = parser.init(); // begin parsing at init rule
        System.out.println(tree.toStringTree(parser)); // print LISP-style tree
    }
}
```





