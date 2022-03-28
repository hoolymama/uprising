"""
Abstracts the process of saving widget values as option vars.
"""
import pymel.core as pm

class PersistentWidget(object):
    def __init__(self, control, var_name, default_value):
        self.control = control
        self.var_name = var_name
        self.default_value = default_value

        def populate(self):
            return NotImplementedError

        def save(self):
            return NotImplementedError

class PersistentCheckBoxGrp(PersistentWidget):

    def save(self):
        pm.optionVar[self.var_name] = pm.checkBoxGrp(self.control, q=True, valueArray4=True)
        
    def populate(self):
        val=pm.optionVar.get(self.var_name, self.default_value)
        
        if not isinstance(val, (list, tuple)):
            val = [val]
        val = (list(val)+[False,False,False,False])[0:4]
        pm.checkBoxGrp(self.control, e=True,valueArray4=val)

class PersistentRadioButtonGrp(PersistentWidget):

    def save(self):
        pm.optionVar[self.var_name] = pm.radioButtonGrp(self.control, q=True, sl=True)
        
    def populate(self):
        val=pm.optionVar.get(self.var_name, self.default_value)
        pm.radioButtonGrp(self.control, e=True,sl=val)

class PersistentIntFieldGrp(PersistentWidget):

    def save(self):
        pm.optionVar[self.var_name] = pm.intFieldGrp(self.control, q=True, value=True)
        
    def populate(self):
        val=pm.optionVar.get(self.var_name, self.default_value)
        if not isinstance(val, (list, tuple)):
            val = [val]
        val = (list(val)+[0,0,0,0])[0:4]

        pm.intFieldGrp(self.control, e=True,value=val)


def factory( owner, control_name, ov_prefix, default_value=None):

    control = getattr(owner, control_name)
    var_name = "{}_{}".format(ov_prefix, control_name)
    widget_type = type(control).__name__

    if widget_type == 'CheckBoxGrp':
        return PersistentCheckBoxGrp(control,var_name, default_value=default_value)
    elif widget_type == 'RadioButtonGrp':
        return PersistentRadioButtonGrp(control,var_name, default_value=default_value)
    elif widget_type == 'IntFieldGrp':
        return PersistentIntFieldGrp(control,var_name, default_value=default_value)
 