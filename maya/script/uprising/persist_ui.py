"""
Abstracts the process of saving/repopulating widget values as option vars.
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

    def reset(self):
        pm.optionVar[self.var_name] = self.default_value
        self.populate()

class PersistentCheckBox(PersistentWidget):

    def save(self):
        pm.optionVar[self.var_name] = pm.checkBox(self.control, q=True, value=True)
        
    def populate(self):
        val=pm.optionVar.get(self.var_name, self.default_value)
        pm.checkBox(self.control, e=True,value=val)

 

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

class PersistentIntSliderGrp(PersistentWidget):

    def save(self):
        pm.optionVar[self.var_name] = pm.intSliderGrp(self.control, q=True, value=True)
        
    def populate(self):
        val=pm.optionVar.get(self.var_name, self.default_value)
        pm.intSliderGrp(self.control, e=True,value=val)

class PersistentIntField(PersistentWidget):

    def save(self):
        pm.optionVar[self.var_name] = pm.intField(self.control, q=True, value=True)
        
    def populate(self):
        val=pm.optionVar.get(self.var_name, self.default_value)
        pm.intField(self.control, e=True,value=val)


class PersistentFloatFieldGrp(PersistentWidget):

    def save(self):
        pm.optionVar[self.var_name] = pm.floatFieldGrp(self.control, q=True, value=True)
        
    def populate(self):
        val=pm.optionVar.get(self.var_name, self.default_value)
        if not isinstance(val, (list, tuple)):
            val = [val]
        val = (list(val)+[0.0,0.0,0.0,0.0])[0:4]

        pm.floatFieldGrp(self.control, e=True,value=val)

class PersistentFloatSliderGrp(PersistentWidget):

    def save(self):
        pm.optionVar[self.var_name] = pm.floatSliderGrp(self.control, q=True, value=True)
        
    def populate(self):
        val=pm.optionVar.get(self.var_name, self.default_value)
        pm.floatSliderGrp(self.control, e=True,value=val)



class PersistentTextFieldGrp(PersistentWidget):

    def save(self):
        pm.optionVar[self.var_name] = pm.textFieldGrp(self.control, q=True, text=True)
        
    def populate(self):
        val=pm.optionVar.get(self.var_name, self.default_value)
        pm.textFieldGrp(self.control, e=True, text=val)


class PersistentTextFieldButtonGrp(PersistentWidget):

    def save(self):
        pm.optionVar[self.var_name] = pm.textFieldButtonGrp(self.control, q=True, text=True)
        
    def populate(self):
        val=pm.optionVar.get(self.var_name, self.default_value)
        pm.textFieldButtonGrp(self.control, e=True, text=val)


class PersistentScrollField(PersistentWidget):

    def save(self):
        pm.optionVar[self.var_name] = pm.scrollField(self.control, q=True, text=True)
        
    def populate(self):
        val=pm.optionVar.get(self.var_name, self.default_value)
        pm.scrollField(self.control, e=True,text=val)

def factory( owner, control_name, ov_prefix, default_value=None):

    control = getattr(owner, control_name)
    var_name = "{}_{}".format(ov_prefix, control_name)
    widget_type = type(control).__name__

    if widget_type == 'CheckBox':
        return PersistentCheckBox(control,var_name, default_value=default_value)
    if widget_type == 'CheckBoxGrp':
        return PersistentCheckBoxGrp(control,var_name, default_value=default_value)
    elif widget_type == 'RadioButtonGrp':
        return PersistentRadioButtonGrp(control,var_name, default_value=default_value)
    elif widget_type == 'IntFieldGrp':
        return PersistentIntFieldGrp(control,var_name, default_value=default_value)
    elif widget_type == 'IntField':
        return PersistentIntField(control,var_name, default_value=default_value)
    elif widget_type == 'TextFieldGrp':
        return PersistentTextFieldGrp(control,var_name, default_value=default_value)
    elif widget_type == 'TextFieldButtonGrp':
        return PersistentTextFieldButtonGrp(control,var_name, default_value=default_value)
    elif widget_type == 'ScrollField':
        return PersistentScrollField(control,var_name, default_value=default_value)
    elif widget_type == 'FloatFieldGrp':
        return PersistentFloatFieldGrp(control,var_name, default_value=default_value)
    elif widget_type == 'IntSliderGrp':
        return PersistentIntSliderGrp(control,var_name, default_value=default_value)
    elif widget_type == 'FloatSliderGrp':
        return PersistentFloatSliderGrp(control,var_name, default_value=default_value)
    
    raise Exception("Unhandled widget type: {}".format(widget_type))