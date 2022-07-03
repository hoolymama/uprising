/** 
 * A brushRack holds brush models.
 * A brushModel holds several brushes of the same with and shape.
 * A stroke that uses a particular brush model, and use any of the brushes in the brush model.
 * 
 * 
 * 
 */



#ifndef _BrushRack_
#define _BrushRack_

#include <string>
#include <map>


// #include <maya/MString.h>

#include "brush.h"
#include "brushModel.h"

class BrushRack {
public:


BrushRack(const std::map<int, Brush> &brushes);

~BrushRack();


std::map<std::string, BrushModel >::const_iterator begin() const;

std::map<std::string, BrushModel >::const_iterator	end() const;

std::map<std::string, BrushModel >::const_iterator  find(const std::string &key) const;

friend ostream &operator<<(ostream &os, const BrushRack &rack);


private:

std::map<std::string, BrushModel > m_brushModels;

};
#endif
