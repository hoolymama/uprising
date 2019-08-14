import io
import os

# from google.cloud import vision
# from google.cloud.vision import types

import pymel.core as pm


def png_palette_for_sheets(fn, sep="\t"):
    # print fn
    palette = pm.pngInfo(fn, palette=True)
    str_palette = [str(x) for x in palette]
    colors = [str_palette[i:i + 4] for i in xrange(0, len(str_palette), 4)]
    name = os.path.splitext(os.path.split(fn)[1])[0]
    print(sep.join([name, "", "", ""]))
    print("\n".join([sep.join(color) for color in colors]))


# def detect_face(fn):

#     client = vision.ImageAnnotatorClient()

#     # with io.open(fn, 'rb') as image_file:
#     #     content = image_file.read()

#     # image = vision.types.Image(content=content)

#     # response = client.face_detection(image=image)
#     # faces = response.face_annotations

#     # # Names of likelihood from google.cloud.vision.enums
#     # likelihood_name = ('UNKNOWN', 'VERY_UNLIKELY', 'UNLIKELY', 'POSSIBLE',
#     #                    'LIKELY', 'VERY_LIKELY')

#     # if not faces:
#     #     pm.displayWarning("Can't find any faces in this image")
#     #     return

#     # if len(faces) > 1:
#     #     pm.displayWarning("Too many faces in this image")
#     #     return

#     # face = faces[0]:

#     # print face.landmarks

#     # print face.fd_bounding_poly

#     # print  face.fd_bounding_poly

#     # vertices = (['({},{})'.format(vertex.x, vertex.y)
#     #              for vertex in face.fd_bounding_poly.vertices])

#     # print('face bounds: {}'.format(','.join(vertices)))
