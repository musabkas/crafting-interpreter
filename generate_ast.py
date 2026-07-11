ExprTypes = [
        ("Assign", [("std::unique_ptr<Token>", "name"), ("std::unique_ptr<Expr>", "value")]), 
        ("Logical", [("std::unique_ptr<Expr>", "left"), ("std::unique_ptr<Token>", "op"), ("std::unique_ptr<Expr>", "right")]), 
        ("Set", [("std::unique_ptr<Expr>", "object"), ("std::unique_ptr<Token>", "name"), ("std::unique_ptr<Expr>", "value")]), 
        ("This", [("std::unique_ptr<Token>", "keyword")]), 
        ("Binary", [("std::unique_ptr<Expr>", "left"), ("std::unique_ptr<Token>", "op"), ("std::unique_ptr<Expr>", "right")]), 
        ("Call", [("std::unique_ptr<Expr>", "callee"), ("std::unique_ptr<Token>", "paren"), ("std::vector<std::unique_ptr<Expr>>", "arguments")]), 
        ("Get", [("std::unique_ptr<Expr>", "object"), ("std::unique_ptr<Token>", "name")]), 
        ("Grouping", [("std::unique_ptr<Expr>", "expression")]),
        ("Literal", [("LoxObject", "value")]),
        ("Unary", [("std::unique_ptr<Token>", "op"), ("std::unique_ptr<Expr>", "right")]),
        ("Variable", [("std::unique_ptr<Token>", "name")]),
        ]
StmtTypes = [
            ("Expression", [("std::unique_ptr<Expr>", "expression")]),
            ("If", [("std::unique_ptr<Expr>", "condition"), ("std::unique_ptr<Stmt>", "thenBranch"), ("std::unique_ptr<Stmt>", "elseBranch")]),
            ("Print", [("std::unique_ptr<Expr>", "expression")]),
            ("Return", [("std::unique_ptr<Token>", "keyword"), ("std::unique_ptr<Expr>", "value")]),
            ("Var", [("std::unique_ptr<Token>", "name"), ("std::unique_ptr<Expr>", "initializer")]),
            ("Function", [("std::unique_ptr<Token>", "name"), ("std::vector<std::unique_ptr<Token>>", "params"), ("std::vector<std::unique_ptr<Stmt>>", "body")]),
            ("While", [("std::unique_ptr<Expr>", "condition"), ("std::unique_ptr<Stmt>", "body")]),
            ("Block", [("std::vector<std::unique_ptr<Stmt>>", "statements")]),
            ("Class", [("std::unique_ptr<Token>", "name"), ("std::unique_ptr<Variable>", "superclass"), ("std::vector<std::unique_ptr<Function>>", "methods")]),
            ]
visitors = [
            # ("ASTPrinter", "std::string"),
            ("Interpreter", "LoxObject"),
            ("Resolver", "void")
            ]


def defineAst(baseName, types, includes = [], loxObjDef = True):
    hppFile = open(baseName + ".hpp", "w")
    cppFile = open(baseName + ".cpp", "w")

    # hpp
    hppFile.writelines([
        '#pragma once\n',
        '#include "Token.hpp"\n',
        '#include <string>\n',
        '#include <memory>\n',
        *[f"#include {inc}\n" for inc in includes],
        "\n",
        ("class LoxCallable;\nclass LoxInstance;\nusing LoxObject = std::variant<double, std::string, bool, std::shared_ptr<LoxCallable>, std::shared_ptr<LoxInstance>, void*>;\n\n") if loxObjDef else "",
        *[f'class {visitor[0]};\n' for visitor in visitors],
        "\n",
        f"class {baseName} {'{'}\n"
        "public:\n",
        f"\tvirtual ~{baseName}() = default;\n",
    ])

    # visitors
    for visitor in visitors:
        hppFile.write(f"\tvirtual {visitor[1]} accept{visitor[0]}({visitor[0]}* visitor) = 0;\n")
    hppFile.write("};\n\n")

    # cpp
    cppFile.write(f'#include "{baseName}.hpp"\n')
    for visitor in visitors:
        cppFile.write(f'#include "{visitor[0]}.hpp"\n')
    cppFile.write('\n')

    # child classes
    for type in types:
        className = type[0]
        fieldList = type[1]
        defineType(hppFile, cppFile, baseName, className, fieldList)


def defineType(hppFile, cppFile, baseName, className, fieldList):
    # print(className, fieldList)
    # hpp
    hppFile.writelines([
        f"class {className} : public {baseName} {'{\n'}",
        "public:\n",
        *[f"\t{f[0]} {f[1]};\n" for f in fieldList],
        "\n",
        f"\t{className}({', '.join([f[0] + ' ' + f[1] for f in fieldList])});\n",
    ])

    # visitors
    for visitor in visitors:
        hppFile.write(f"\t{visitor[1]} accept{visitor[0]}({visitor[0]}* visitor) override;\n")
    hppFile.write("};\n\n")

    # cpp
    cppFile.writelines([
        f"{className}::{className}({', '.join([f[0] + ' ' + f[1] for f in fieldList])}){'{'}\n",
        *[f"\tthis->{f[1]} = {f'std::move({f[1]})' if ("std::unique_ptr" in f[0]) else f[1] };\n" for f in fieldList],
        "}\n\n"
    ])

    # visitors
    for visitor in visitors:
        cppFile.write(f"{visitor[1]} {className}::accept{visitor[0]}({visitor[0]}* visitor){'{'}\n")
        cppFile.write(f"\treturn visitor->visit{className}(this);\n")
        cppFile.write("}\n\n")


includes = ["<vector>"]
defineAst("Expr", ExprTypes, includes)
visitors = [
            ("Interpreter", "void"),
            ("Resolver", "void")
            ]
includes = ["<vector>", '"Expr.hpp"']
defineAst("Stmt", StmtTypes, includes, False)