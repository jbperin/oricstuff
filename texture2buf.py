
from PIL import Image
import codegen

im = Image.open('img.bmp')
rgb_im = im.convert('RGB')
imw, imh = im.width, im.height

def toSimpleRgb(val):
    return val//85

bufimg = []

for jj in range (imh):
    for ii in range (imw):    
        r, g, b = rgb_im.getpixel((ii, jj))
        bufimg.append(toSimpleRgb(r)*16 + toSimpleRgb(g)*4 + toSimpleRgb(b))
        print (r, g, b, toSimpleRgb(r), toSimpleRgb(g), toSimpleRgb(b), toSimpleRgb(r)*16 + toSimpleRgb(g)*4 + toSimpleRgb(b))

# print(buf_blue)

cCode = f"#define TEXTURE_WIDTH {imw}\n#define TEXTURE_HEIGHT {imh}\n"


cCode += codegen.buffer2cCode("bufimg", bufimg)
print (cCode)