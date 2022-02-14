
file = "skull_1"


def cast_vert(vert: str) -> str:
    s = []
    for c in vert:
        if c == "/":
            break
        s.append(c)
    return ''.join(s)


if __name__ == "__main__":

    with open(f"../obj/{file}.obj", "r") as fin:

        lines = fin.readlines()

        filtered_lines = []
        for line in lines:
            if line.startswith("v "):
                filtered_lines.append(line)
            elif line.startswith("f "):
                casted_line = " ".join(
                    map(
                        lambda x: cast_vert(x),
                        line.split(" ")
                    )
                )
                if casted_line[-1] != '\n':
                    casted_line += casted_line + '\n'
                filtered_lines.append(casted_line)

        filtered_lines.sort(key=lambda x: x[0], reverse=True)
        # print(filtered_lines)

    with open(f"../obj/{file}Casted.obj", "w") as fout:
        fout.writelines(filtered_lines)
