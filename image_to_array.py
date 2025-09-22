from PIL import Image
import sys


def main() -> None:
    if len(sys.argv) != 2:
        print("Usage: python image_to_array.py <image_file>")
        sys.exit(1)

    image_file_path = sys.argv[1]
    img = Image.open(image_file_path).convert("RGB")
    colors: list[tuple[int, int, int]] = list(img.getdata())

    int_array: list[int] = []

    for color in colors:
        color = ((color[0] & 0b11111000) << 8) | ((color[1] & 0b11111100) << 3) | (color[2] >> 3)
        # swap bytes
        color = ((color & 0xFF) << 8) | ((color >> 8) & 0xFF)
        int_array.append(color)

    with open("image.c", "w") as outFile:
        outFile.write("#include <stdint.h>\n\n")
        outFile.write("const uint16_t image_data[] = {\n")
        
        index = 0
        while index < len(int_array):
            line_data = int_array[index:index + 12]
            outFile.write(" " * 4 + ", ".join(map(lambda x: f"0x{x:04X}", line_data)) + ",\n")
            index += 12

        outFile.write("};\n")

    print("Output written to image.c")

if __name__ == "__main__":
    main()
