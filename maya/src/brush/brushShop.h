/**
 * A brushShop holds brush models.
 * A brushModel holds several brushes of the same width and shape.
 * A stroke that uses a particular brush model can use any of the brushes in the brush model.
 *
 */

#ifndef _BrushShop_
#define _BrushShop_

#include <map>

#include "brush.h"
#include "brushRack.h"

class BrushShop
{
public:
    BrushShop();

    ~BrushShop();

	std::map<Brush::Shape, BrushRack> racks;

    void insertRack(const std::pair<Brush::Shape, BrushRack> &rack );
    

    friend ostream &operator<<(ostream &os, const BrushShop &shop);
 
    void getBrushes(std::map<int, Brush> & brushes) const;

 
    

private:



};
#endif
