
import pymel.core as pm

from uprising import props
import pymel.core as pm
from uprising import robo



def create():
    menu = pm.menu(label="KR8 Particles", tearOff=True)

    pm.menuItem(label="Create Image Gradient Field",
                command=pm.Callback(create_gradient_image_field))

    return menu


def create_gradient_image_field():

    # The thing we'll add the field to
    target = pm.ls(sl=True)[0]

    field=pm.createNode("cImgGradField")

    pm.connectDynamic(target, f=field)

    grad=pm.createNode("cImgGradient") 
    grad.attr("output") >> field.attr("inputGradientImage")

    for val in [0,1]:
        for att in ["Position", "FloatValue"]:
            field.attr("magnitudeRemapRamp[{}].magnitudeRemapRamp_{}".format(val,att)).set(val)

    pm.select(field)
 