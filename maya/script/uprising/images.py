import io
import os

 
import pymel.core as pm


def png_palette_for_sheets(fn, sep="\t"):
 
    palette = pm.pngInfo(fn, palette=True)
    str_palette = [str(x) for x in palette]
    colors = [str_palette[i:i + 4] for i in xrange(0, len(str_palette), 4)]
    name = os.path.splitext(os.path.split(fn)[1])[0]
    print(sep.join([name, "", "", ""]))
    print("\n".join([sep.join(color) for color in colors]))

