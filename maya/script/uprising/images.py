import os
import pymel.core as pm
 
# from paint import Paint
# from brush import Brush
# import curve_utils as cutl
# import uprising.uprising_util as uutl
# import logging
# logger = logging.getLogger('uprising')



def png_palette_for_sheets(fn, sep="\t"):
    print fn
    num = pm.pngInfo(fn, palleteSize=True)
    palette = pm.pngInfo(fn, palette=True)
    str_palette = [str(x) for x in palette]
    colors = [str_palette[i:i + 4] for i in xrange(0, len(str_palette), 4)]
    name =   os.path.splitext(os.path.split(fn)[1])[0]
    print sep.join([name, "", "", ""])
    print "\n".join([sep.join(color) for color in colors])
