types = [("Binary", [("std::unique_ptr<Expr>", "left"), ("std::unique_ptr<Token>", "op"), ("std::unique_ptr<Expr>", "right")]), 
        ("Grouping", [("std::unique_ptr<Expr>", "expression")]),
        ("Literal", [("LoxObject", "value")]),
        ("Unary", [("std::unique_ptr<Token>", "op"), ("std::unique_ptr<Expr>", "right")])]
StmtTypes = [("Expression", [("std::unique_ptr<Expr>", "expression")]),
            ("Print", [("std::unique_ptr<Expr>", "expression")])]
visitors = [("ASTPrinter", "std::string"),
            ("Interpreter", "LoxObject")]


def defineAst(baseName, types, includes = []):
    hppFile = open(baseName + ".hpp", "w")
    cppFile = open(baseName + ".cpp", "w")

    # hpp
    hppFile.writelines([
        '#pragma once\n',
        '#include "Token.hpp"\n',
        '#include <string>\n',
        '#include <memory>\n',
        *[f"#include {inc}" for inc in includes],
        "\n",
        "using LoxObject = std::variant<double, std::string, bool, void*>;\n",
        "\n",
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


defineAst("Expr", types)
visitors = [("Interpreter", "void")]
includes = ['"Expr.hpp"']
defineAst("Stmt", StmtTypes, includes)