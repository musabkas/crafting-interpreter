baseName = "Expr"
types = [("Binary", [("Expr*", "left"), ("Token*", "op"), ("Expr*", "right")]), 
        ("Grouping", [("Expr*", "expression")]),
        ("Literal", [("std::variant<double, std::string, void *>", "value")]),
        ("Unary", [("Token*", "op"), ("Expr*", "right")])]
visitors = [("ASTPrinter", "std::string")]


def defineAst(baseName, types):
    hppFile = open(baseName + ".hpp", "w")
    cppFile = open(baseName + ".cpp", "w")

    # hpp
    hppFile.writelines([
        '#pragma once\n',
        '#include "Token.hpp"\n',
        '#include <string>\n',
        "\n",
        'class ASTPrinter;\n\n',
        f"class {baseName} {'{'}\n"
        "public:\n"
    ])

    # visitors
    for visitor in visitors:
        hppFile.write(f"\tvirtual {visitor[1]} accept{visitor[0]}({visitor[0]}* visitor) = 0;\n")
    hppFile.write("};\n\n")

    # cpp
    cppFile.write('#include "Expr.hpp"\n')
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
        *[f"\tthis->{f[1]} = {f[1]};\n" for f in fieldList],
        "}\n\n"
    ])

    # visitors
    for visitor in visitors:
        cppFile.write(f"{visitor[1]} {className}::accept{visitor[0]}({visitor[0]}* visitor){'{'}\n")
        cppFile.write(f"\treturn visitor->visit{className}(this);\n")
        cppFile.write("}\n\n")


defineAst(baseName, types)