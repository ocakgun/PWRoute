#include "defDataBase.h"
#include "lefDataBase.h"
#include "global.h"
#include "grGen.h"

#include "fastroute.h"
#include "maxflow.h"
#include "pwgnd_route.h"
using namespace parser;

void computePinLocation(parser::Component& component)
{
    int llx, lly, urx, ury;
    if(component.orient == "N")
    {
        for(auto& pin : component.macro.pins)
        {
            for(auto& layerRect : pin.layerRects)
            {
                for(auto& rect : layerRect.rects)
                {
                    llx = component.location.x + component.macro.origin.x + rect.lowerLeft.x;
                    lly = component.location.y + component.macro.origin.y + rect.lowerLeft.y;
                    urx = component.location.x + component.macro.origin.x + rect.upperRight.x;
                    ury = component.location.y + component.macro.origin.y + rect.upperRight.y;
                    rect.set(llx, lly, urx, ury);
                }
            }
        }
    }
    else if(component.orient == "S")
    {
        for(auto& pin : component.macro.pins)
        {
            for(auto& layerRect : pin.layerRects)
            {
                for(auto& rect : layerRect.rects)
                {
                    llx = component.location.x + component.macro.origin.x + component.macro.size.x - rect.upperRight.x;
                    lly = component.location.y + component.macro.origin.y + component.macro.size.y - rect.upperRight.y;
                    urx = component.location.x + component.macro.origin.x + component.macro.size.x - rect.lowerLeft.x;
                    ury = component.location.y + component.macro.origin.y + component.macro.size.y - rect.lowerLeft.y;
                    rect.set(llx, lly, urx, ury);
                }
            }
        }

    }
    else if(component.orient == "E")
    {
        for(auto& pin : component.macro.pins)
        {
            for(auto& layerRect : pin.layerRects)
            {
                for(auto& rect : layerRect.rects)
                {
                    float centerx = (rect.lowerLeft.x + rect.upperRight.x)/2.0;
                    float centery = (rect.lowerLeft.y + rect.upperRight.y)/2.0;
                    int rectwidth = rect.upperRight.x - rect.lowerLeft.x;
                    int rectheight = rect.upperRight.y - rect.lowerLeft.y;
                    float centerxprime =  centery; 
                    // assumption origin.x and origin.y is always 0
                    // cos(270deg)centerx - sin(270deg)centery
                    float centeryprime = -centerx; 
                    // sin(270deg)centerx + cos(270deg)centery
                    int tmplength;
                    tmplength = rectwidth; 
                    // 270deg rotation, width and height swap
                    rectwidth = rectheight;
                    rectheight = tmplength; 
                    // macro width and height should also swap, but we cannot do it here, because we are at pin level now
                    // but remember, when we need macro width, we should call its height, and vice versa
                    llx = centerxprime - rectwidth/2.0;
                    lly = centeryprime - rectheight/2.0;
                    urx = centerxprime + rectwidth/2.0;
                    ury = centeryprime + rectheight/2.0;
                    // shift upward for a macro height, now we should use macro.size.x, after shift move to (comp.placeLoc.x, comp.placeLoc.y)
                    llx += component.location.x;
                    lly += component.macro.size.x + component.location.y;
                    urx += component.location.x;
                    ury += component.macro.size.x + component.location.y;

                    llx += component.macro.origin.x;
                    lly += component.macro.origin.y;
                    urx += component.macro.origin.x;
                    ury += component.macro.origin.y;
                    rect.set(llx, lly, urx, ury);
                }
            }
        }

    }
    else if(component.orient == "W")
    {
        for(auto& pin : component.macro.pins)
        {
            for(auto& layerRect : pin.layerRects)
            {
                for(auto& rect : layerRect.rects)
                {
                    float centerx = (rect.lowerLeft.x + rect.upperRight.x)/2.0;
                    float centery = (rect.lowerLeft.y + rect.upperRight.y)/2.0;
                    int rectwidth = rect.upperRight.x - rect.lowerLeft.x;
                    int rectheight = rect.upperRight.y - rect.lowerLeft.y;
                    float centerxprime = -centery; 
                    // assumption origin.x and origin.y is always 0
                    // cos(90deg)centerx - sin(90deg)centery
                    float centeryprime =  centerx; 
                    // sin(90deg)centerx + cos(90deg)centery
                    int tmplength;
                    tmplength = rectwidth; 
                    // 90deg rotation, width and height swap
                    rectwidth = rectheight;
                    rectheight = tmplength; 
                    // macro width and height should also swap, but we cannot do it here, because we are at pin level now
                    // but remember, when we need macro width, we should call its height, and vice versa
                    llx = std::round(centerxprime - rectwidth/2.0);
                    lly = std::round(centeryprime - rectheight/2.0);
                    urx = std::round(centerxprime + rectwidth/2.0);
                    ury = std::round(centeryprime + rectheight/2.0);
                    // shift rightward for a macro width, now we should use macro.size.y, after shift move to (comp.placeLoc.x, comp.placeLoc.y)
                    llx += component.macro.size.y + component.location.x;
                    lly += component.location.y;
                    urx += component.macro.size.y + component.location.x;
                    ury += component.location.y;


                    llx += component.macro.origin.x;
                    lly += component.macro.origin.y;
                    urx += component.macro.origin.x;
                    ury += component.macro.origin.y;
                    rect.set(llx, lly, urx, ury);
                }
            }
        }

    }
    else if(component.orient == "FN")
    {
        for(auto& pin : component.macro.pins)
        {
            for(auto& layerRect : pin.layerRects)
            {
                for(auto& rect : layerRect.rects)
                {
                    llx = component.location.x + component.macro.size.x - rect.upperRight.x;
                    lly = component.location.y + rect.lowerLeft.y;
                    urx = component.location.x + component.macro.size.x - rect.lowerLeft.x;
                    ury = component.location.y + rect.upperRight.y;

                    llx += component.macro.origin.x;
                    lly += component.macro.origin.y;
                    urx += component.macro.origin.x;
                    ury += component.macro.origin.y;
                    rect.set(llx, lly, urx, ury);
                }
            }
        }

    }
    else if(component.orient == "FS")
    {
        for(auto& pin : component.macro.pins)
        {
            for(auto& layerRect : pin.layerRects)
            {
                for(auto& rect : layerRect.rects)
                {
                    llx = component.location.x + rect.lowerLeft.x;
                    lly = component.location.y + component.macro.size.y - rect.upperRight.y;
                    urx = component.location.x + rect.upperRight.x;
                    ury = component.location.y + component.macro.size.y - rect.lowerLeft.y;

                    llx += component.macro.origin.x;
                    lly += component.macro.origin.y;
                    urx += component.macro.origin.x;
                    ury += component.macro.origin.y;
                    rect.set(llx, lly, urx, ury);
                }
            }
        }

    }
    else if(component.orient == "FE")
    {
        for(auto& pin : component.macro.pins)
        {
            for(auto& layerRect : pin.layerRects)
            {
                for(auto& rect : layerRect.rects)
                {
                    float centerx = (rect.lowerLeft.x + rect.upperRight.x)/2.0;
                    float centery = (rect.lowerLeft.y + rect.upperRight.y)/2.0;
                    int rectwidth = rect.upperRight.x - rect.lowerLeft.x;
                    int rectheight = rect.upperRight.y - rect.lowerLeft.y;
                    float centerxprime =  centery; 
                    // assumption origin.x and origin.y is always 0
                    // cos(270deg)centerx - sin(270deg)centery
                    float centeryprime = -centerx; 
                    // sin(270deg)centerx + cos(270deg)centery
                    int tmplength;
                    tmplength = rectwidth; 
                    // 270deg rotation, width and height swap
                    rectwidth = rectheight;
                    rectheight = tmplength; 
                    // macro width and height should also swap, but we cannot do it here, because we are at pin level now
                    // but remember, when we need macro width, we should call its height, and vice versa
                    llx = centerxprime - rectwidth/2.0;
                    lly = centeryprime - rectheight/2.0;
                    urx = centerxprime + rectwidth/2.0;
                    ury = centeryprime + rectheight/2.0;
                    // shift upward for a macro height, now we should use macro.size.x
                    lly += component.macro.size.x;
                    ury += component.macro.size.x;

                    int axis = component.macro.size.y;
                    centerx = (llx + urx)/2.0;
                    centery = (lly + ury)/2.0;
                    rectwidth = urx - llx;
                    rectheight = ury - lly;

                    centerx = axis - centerx;

                    llx = std::round(centerx - rectwidth/2.0) + component.location.x;
                    lly = std::round(centery - rectheight/2.0) + component.location.y;
                    urx = std::round(centerx + rectwidth/2.0) + component.location.x;
                    ury = std::round(centery + rectheight/2.0) + component.location.y;

                    llx += component.macro.origin.x;
                    lly += component.macro.origin.y;
                    urx += component.macro.origin.x;
                    ury += component.macro.origin.y;

                    rect.set(llx, lly, urx, ury);
                }
            }
        }

    }
    else if(component.orient == "FW")
    {
        for(auto& pin : component.macro.pins)
        {
            for(auto& layerRect : pin.layerRects)
            {
                for(auto& rect : layerRect.rects)
                {
                    float centerx = (rect.lowerLeft.x + rect.upperRight.x)/2.0;
                    float centery = (rect.lowerLeft.y + rect.upperRight.y)/2.0;
                    int rectwidth = rect.upperRight.x - rect.lowerLeft.x;
                    int rectheight = rect.upperRight.y - rect.lowerLeft.y;
                    float centerxprime = -centery; 
                    // assumption origin.x and origin.y is always 0
                    // cos(90deg)centerx - sin(90deg)centery
                    float centeryprime =  centerx; 
                    // sin(90deg)centerx + cos(90deg)centery
                    int tmplength;
                    tmplength = rectwidth; 
                    // 90deg rotation, width and height swap
                    rectwidth = rectheight;
                    rectheight = tmplength; 
                    // macro width and height should also swap, but we cannot do it here, because we are at pin level now
                    // but remember, when we need macro width, we should call its height, and vice versa
                    llx = std::round(centerxprime - rectwidth/2.0);
                    lly = std::round(centeryprime - rectheight/2.0);
                    urx = std::round(centerxprime + rectwidth/2.0);
                    ury = std::round(centeryprime + rectheight/2.0);
                    // shift rightward for a macro width, now we should use macro.size.y
                    llx += component.macro.size.y;
                    urx += component.macro.size.y;
                    // flip, this is done using the right boundary location, which is macro.size.y
                    int axis = component.macro.size.y;
                    centerx = (llx + urx)/2.0;
                    centery = (lly + ury)/2.0;
                    rectwidth = urx - llx;
                    rectheight = ury - lly;

                    centerx = axis - centerx;

                    llx = std::round(centerx - rectwidth/2.0) + component.location.x;
                    lly = std::round(centery - rectheight/2.0) + component.location.y;
                    urx = std::round(centerx + rectwidth/2.0) + component.location.x;
                    ury = std::round(centery + rectheight/2.0) + component.location.y;

                    llx += component.macro.origin.x;
                    lly += component.macro.origin.y;
                    urx += component.macro.origin.x;
                    ury += component.macro.origin.y;

                    rect.set(llx, lly, urx, ury);
                }
            }
        }

    }
    else
    {
        cout << "unknown orientation for component: " << component.idx << " " << component.name << " " << component.orient << endl;
        exit(1);
    }
}


void computeOBSLocation(parser::Component& component)
{
    int llx, lly, urx, ury;
    if(component.orient == "N")
    {
        for(auto& layerRect : component.macro.obs.layerRects)
        {
            for(auto& rect : layerRect.rects)
            {
                llx = component.location.x + component.macro.origin.x + rect.lowerLeft.x;
                lly = component.location.y + component.macro.origin.y + rect.lowerLeft.y;
                urx = component.location.x + component.macro.origin.x + rect.upperRight.x;
                ury = component.location.y + component.macro.origin.y + rect.upperRight.y;
                rect.set(llx, lly, urx, ury);
            }
        }
    }
    else if(component.orient == "S")
    {

            for(auto& layerRect : component.macro.obs.layerRects)
            {
                for(auto& rect : layerRect.rects)
                {
                    llx = component.location.x + component.macro.origin.x + component.macro.size.x - rect.upperRight.x;
                    lly = component.location.y + component.macro.origin.y + component.macro.size.y - rect.upperRight.y;
                    urx = component.location.x + component.macro.origin.x + component.macro.size.x - rect.lowerLeft.x;
                    ury = component.location.y + component.macro.origin.y + component.macro.size.y - rect.lowerLeft.y;
                    rect.set(llx, lly, urx, ury);
                }
            }

    }
    else if(component.orient == "E")
    {
            for(auto& layerRect : component.macro.obs.layerRects)
            {
                for(auto& rect : layerRect.rects)
                {
                    float centerx = (rect.lowerLeft.x + rect.upperRight.x)/2.0;
                    float centery = (rect.lowerLeft.y + rect.upperRight.y)/2.0;
                    int rectwidth = rect.upperRight.x - rect.lowerLeft.x;
                    int rectheight = rect.upperRight.y - rect.lowerLeft.y;
                    float centerxprime =  centery; 
                    // assumption origin.x and origin.y is always 0
                    // cos(270deg)centerx - sin(270deg)centery
                    float centeryprime = -centerx; 
                    // sin(270deg)centerx + cos(270deg)centery
                    int tmplength;
                    tmplength = rectwidth; 
                    // 270deg rotation, width and height swap
                    rectwidth = rectheight;
                    rectheight = tmplength; 
                    // macro width and height should also swap, but we cannot do it here, because we are at pin level now
                    // but remember, when we need macro width, we should call its height, and vice versa
                    llx = centerxprime - rectwidth/2.0;
                    lly = centeryprime - rectheight/2.0;
                    urx = centerxprime + rectwidth/2.0;
                    ury = centeryprime + rectheight/2.0;
                    // shift upward for a macro height, now we should use macro.size.x, after shift move to (comp.placeLoc.x, comp.placeLoc.y)
                    llx += component.location.x;
                    lly += component.macro.size.x + component.location.y;
                    urx += component.location.x;
                    ury += component.macro.size.x + component.location.y;

                    llx += component.macro.origin.x;
                    lly += component.macro.origin.y;
                    urx += component.macro.origin.x;
                    ury += component.macro.origin.y;
                    rect.set(llx, lly, urx, ury);
                }
            }

    }
    else if(component.orient == "W")
    {
            for(auto& layerRect : component.macro.obs.layerRects)
            {
                for(auto& rect : layerRect.rects)
                {
                    float centerx = (rect.lowerLeft.x + rect.upperRight.x)/2.0;
                    float centery = (rect.lowerLeft.y + rect.upperRight.y)/2.0;
                    int rectwidth = rect.upperRight.x - rect.lowerLeft.x;
                    int rectheight = rect.upperRight.y - rect.lowerLeft.y;
                    float centerxprime = -centery; 
                    // assumption origin.x and origin.y is always 0
                    // cos(90deg)centerx - sin(90deg)centery
                    float centeryprime =  centerx; 
                    // sin(90deg)centerx + cos(90deg)centery
                    int tmplength;
                    tmplength = rectwidth; 
                    // 90deg rotation, width and height swap
                    rectwidth = rectheight;
                    rectheight = tmplength; 
                    // macro width and height should also swap, but we cannot do it here, because we are at pin level now
                    // but remember, when we need macro width, we should call its height, and vice versa
                    llx = std::round(centerxprime - rectwidth/2.0);
                    lly = std::round(centeryprime - rectheight/2.0);
                    urx = std::round(centerxprime + rectwidth/2.0);
                    ury = std::round(centeryprime + rectheight/2.0);
                    // shift rightward for a macro width, now we should use macro.size.y, after shift move to (comp.placeLoc.x, comp.placeLoc.y)
                    llx += component.macro.size.y + component.location.x;
                    lly += component.location.y;
                    urx += component.macro.size.y + component.location.x;
                    ury += component.location.y;


                    llx += component.macro.origin.x;
                    lly += component.macro.origin.y;
                    urx += component.macro.origin.x;
                    ury += component.macro.origin.y;
                    rect.set(llx, lly, urx, ury);
                }
            }

    }
    else if(component.orient == "FN")
    {
            for(auto& layerRect : component.macro.obs.layerRects)
            {
                for(auto& rect : layerRect.rects)
                {
                    llx = component.location.x + component.macro.size.x - rect.upperRight.x;
                    lly = component.location.y + rect.lowerLeft.y;
                    urx = component.location.x + component.macro.size.x - rect.lowerLeft.x;
                    ury = component.location.y + rect.upperRight.y;

                    llx += component.macro.origin.x;
                    lly += component.macro.origin.y;
                    urx += component.macro.origin.x;
                    ury += component.macro.origin.y;
                    rect.set(llx, lly, urx, ury);
                }
            }

    }
    else if(component.orient == "FS")
    {
            for(auto& layerRect : component.macro.obs.layerRects)
            {
                for(auto& rect : layerRect.rects)
                {
                    llx = component.location.x + rect.lowerLeft.x;
                    lly = component.location.y + component.macro.size.y - rect.upperRight.y;
                    urx = component.location.x + rect.upperRight.x;
                    ury = component.location.y + component.macro.size.y - rect.lowerLeft.y;

                    llx += component.macro.origin.x;
                    lly += component.macro.origin.y;
                    urx += component.macro.origin.x;
                    ury += component.macro.origin.y;
                    rect.set(llx, lly, urx, ury);
                }
            }

    }
    else if(component.orient == "FE")
    {
            for(auto& layerRect : component.macro.obs.layerRects)
            {
                for(auto& rect : layerRect.rects)
                {
                    float centerx = (rect.lowerLeft.x + rect.upperRight.x)/2.0;
                    float centery = (rect.lowerLeft.y + rect.upperRight.y)/2.0;
                    int rectwidth = rect.upperRight.x - rect.lowerLeft.x;
                    int rectheight = rect.upperRight.y - rect.lowerLeft.y;
                    float centerxprime =  centery; 
                    // assumption origin.x and origin.y is always 0
                    // cos(270deg)centerx - sin(270deg)centery
                    float centeryprime = -centerx; 
                    // sin(270deg)centerx + cos(270deg)centery
                    int tmplength;
                    tmplength = rectwidth; 
                    // 270deg rotation, width and height swap
                    rectwidth = rectheight;
                    rectheight = tmplength; 
                    // macro width and height should also swap, but we cannot do it here, because we are at pin level now
                    // but remember, when we need macro width, we should call its height, and vice versa
                    llx = centerxprime - rectwidth/2.0;
                    lly = centeryprime - rectheight/2.0;
                    urx = centerxprime + rectwidth/2.0;
                    ury = centeryprime + rectheight/2.0;
                    // shift upward for a macro height, now we should use macro.size.x
                    lly += component.macro.size.x;
                    ury += component.macro.size.x;

                    int axis = component.macro.size.y;
                    centerx = (llx + urx)/2.0;
                    centery = (lly + ury)/2.0;
                    rectwidth = urx - llx;
                    rectheight = ury - lly;

                    centerx = axis - centerx;

                    llx = std::round(centerx - rectwidth/2.0) + component.location.x;
                    lly = std::round(centery - rectheight/2.0) + component.location.y;
                    urx = std::round(centerx + rectwidth/2.0) + component.location.x;
                    ury = std::round(centery + rectheight/2.0) + component.location.y;

                    llx += component.macro.origin.x;
                    lly += component.macro.origin.y;
                    urx += component.macro.origin.x;
                    ury += component.macro.origin.y;

                    rect.set(llx, lly, urx, ury);
                }
            }
    }
    else if(component.orient == "FW")
    {
            for(auto& layerRect : component.macro.obs.layerRects)
            {
                for(auto& rect : layerRect.rects)
                {
                    float centerx = (rect.lowerLeft.x + rect.upperRight.x)/2.0;
                    float centery = (rect.lowerLeft.y + rect.upperRight.y)/2.0;
                    int rectwidth = rect.upperRight.x - rect.lowerLeft.x;
                    int rectheight = rect.upperRight.y - rect.lowerLeft.y;
                    float centerxprime = -centery; 
                    // assumption origin.x and origin.y is always 0
                    // cos(90deg)centerx - sin(90deg)centery
                    float centeryprime =  centerx; 
                    // sin(90deg)centerx + cos(90deg)centery
                    int tmplength;
                    tmplength = rectwidth; 
                    // 90deg rotation, width and height swap
                    rectwidth = rectheight;
                    rectheight = tmplength; 
                    // macro width and height should also swap, but we cannot do it here, because we are at pin level now
                    // but remember, when we need macro width, we should call its height, and vice versa
                    llx = std::round(centerxprime - rectwidth/2.0);
                    lly = std::round(centeryprime - rectheight/2.0);
                    urx = std::round(centerxprime + rectwidth/2.0);
                    ury = std::round(centeryprime + rectheight/2.0);
                    // shift rightward for a macro width, now we should use macro.size.y
                    llx += component.macro.size.y;
                    urx += component.macro.size.y;
                    // flip, this is done using the right boundary location, which is macro.size.y
                    int axis = component.macro.size.y;
                    centerx = (llx + urx)/2.0;
                    centery = (lly + ury)/2.0;
                    rectwidth = urx - llx;
                    rectheight = ury - lly;

                    centerx = axis - centerx;

                    llx = std::round(centerx - rectwidth/2.0) + component.location.x;
                    lly = std::round(centery - rectheight/2.0) + component.location.y;
                    urx = std::round(centerx + rectwidth/2.0) + component.location.x;
                    ury = std::round(centery + rectheight/2.0) + component.location.y;

                    llx += component.macro.origin.x;
                    lly += component.macro.origin.y;
                    urx += component.macro.origin.x;
                    ury += component.macro.origin.y;

                    rect.set(llx, lly, urx, ury);
                }
            }

    }
    else
    {
        cout << "unknown orientation for component: " << component.idx << " " << component.name << " " << component.orient << endl;
        exit(1);
    }
}

void preprocessComponent( )
{
    int dbuPerMicro = defDB.dbuPerMicro;
    defDB.origOBS.resize(lefDB.layers.size());
    defDB.designRuleOBS.resize(lefDB.layers.size());
    for(auto& component : defDB.components)
    {   
        assert(lefDB.macro2idx.find(component.macroName) != lefDB.macro2idx.end());

        int macroIdx = lefDB.macro2idx.find(component.macroName)->second;
        component.macro = lefDB.macros.at(macroIdx);

        component.macro.origin.x *= dbuPerMicro;
        component.macro.origin.y *= dbuPerMicro;

        component.macro.size.x *= dbuPerMicro;
        component.macro.size.y *= dbuPerMicro;

        for(auto& pin : component.macro.pins)
        {
            for(auto& layerRect : pin.layerRects)
            {
                for(auto& rect : layerRect.rects)
                {
                    rect.lowerLeft.x *= dbuPerMicro;
                    rect.lowerLeft.y *= dbuPerMicro;
                    rect.upperRight.x *= dbuPerMicro;
                    rect.upperRight.y *= dbuPerMicro;
                }
            }
        }


        for(auto& layerRect : component.macro.obs.layerRects)
        {
            for(auto& rect : layerRect.rects)
            {
                rect.lowerLeft.x *= dbuPerMicro;
                rect.lowerLeft.y *= dbuPerMicro;
                rect.upperRight.x *= dbuPerMicro;
                rect.upperRight.y *= dbuPerMicro;
            }
        }
    
        computePinLocation(component);
        computeOBSLocation(component);

        //push OBS in record
        for(auto pin : component.macro.pins)
        {
            if(pin.use == "POWER" || pin.name == "GROUND") 
            {
                for(auto layerRect : pin.layerRects)
                {
                    int layer = lefDB.layer2idx.find(layerRect.layerName)->second;
                    if(layer > 0)
                    {
                        for(auto rect : layerRect.rects)
                        {
                            defDB.origOBS.at(layer).push_back(rect);
                            /*if(component.name == "inst15277")
                            {
                                cout << component.name << "/" << pin.name << endl;
                                cout << rect << endl;
                            }*/
                        }
                    }   
                }
            }
            if(pin.use == "POWER") {
                bool newNet = false;
               if(defDB.powerNet2idx.find(pin.name) == defDB.powerNet2idx.end()) {
                    defDB.powerNet2idx[pin.name] = defDB.powerNets.size();
                    defDB.powerNets.push_back(pin.name);
                    newNet = true;
                } 
               
               Point2D<int> center;
                center.x = (pin.layerRects[0].rects[0].lowerLeft.x + pin.layerRects[0].rects[0].upperRight.x) / 2;
                center.y = (pin.layerRects[0].rects[0].lowerLeft.y + pin.layerRects[0].rects[0].upperRight.y) / 2;
                int powerID = defDB.powerNet2idx[pin.name];
                if(newNet) {
                 vector<Point2D<int>> newNetVDD;
                 newNetVDD.push_back(center);
                 defDB.pwgnd.VDDpins.push_back(newNetVDD);
                }
                else 
                    defDB.pwgnd.VDDpins[powerID].push_back(center);   
            }

            else if(pin.use == "GROUND") {
               bool newNet = false;
                if(defDB.gndNet2idx.find(pin.name) == defDB.gndNet2idx.end()) {
                    defDB.gndNet2idx[pin.name] = defDB.gndNets.size();
                    defDB.gndNets.push_back(pin.name);
                    newNet = true;
                } 
               
               Point2D<int> center;
                center.x = (pin.layerRects[0].rects[0].lowerLeft.x + pin.layerRects[0].rects[0].upperRight.x) / 2;
                center.y = (pin.layerRects[0].rects[0].lowerLeft.y + pin.layerRects[0].rects[0].upperRight.y) / 2;
                int gndID = defDB.gndNet2idx[pin.name];
                if(newNet) {
                    vector<Point2D<int>> newNetGND;
                    newNetGND.push_back(center);
                    defDB.pwgnd.GNDpins.push_back(newNetGND);
                }
                else 
                    defDB.pwgnd.GNDpins[gndID].push_back(center); 
            
            }
        }

        for(auto layerRect : component.macro.obs.layerRects)
        {
            int layer = lefDB.layer2idx.find(layerRect.layerName)->second;
            if(layer > 0)
            {   
                for(auto rect : layerRect.rects)
                {
                    defDB.origOBS.at(layer).push_back(rect);
                    //rect.print();
                }
            }
        }
    }

}



std::stringstream removeComments(ifstream& infile)
{
    stringstream fileStream("");
    string line;
    while(getline(infile, line))
    {
        //cout << "removing comments" << endl;
        if(line.find("#") != std::string::npos)
        {
            //cout << line << endl;
            //cout << line.find("#") << " " << line.size() << endl;
            size_t pos = line.find("#");
            line.erase(line.begin() + pos, line.end()); 
        }
        line.append("\n");
        fileStream << line;
    }
    return fileStream;
}

/*void readDef(string defFileName )
{
    //string filename("ispd18_test1.input.def");

    ifstream infile(defFileName);
    if(!infile.is_open()) {
        cout << "Unable to open file: " << defFileName << endl;
        exit(1);
    }
    
    std::stringstream defStream;
    defStream = removeComments(infile);
    
    int cnt = 0;
    string token, token1, token2;
    string comment;
    bool endOfDesign = false;
    while(!defStream.eof())
    {
        defStream >> token;
        //   cout << token << endl;
        if(token.at(0) == '#')
        {
            getline(defStream, comment);
            continue;
        }
        defKeyword keyword = token2defKeyword(token);
        
        switch(keyword) {
            case VERSION: 
                defStream >> defDB.version;
                cout << defDB.version << endl;
                semicolonCheck(defStream);
                break;
            case DIVIDERCHAR:
                defStream >> defDB.dividerChar;
                cout << defDB.dividerChar << endl;
                semicolonCheck(defStream);
                break;
            case BUSBITCHARS:
                defStream >> defDB.busBitChars;
                cout << defDB.busBitChars << endl;
                semicolonCheck(defStream);
                break;
            case DESIGN:
                defStream >> defDB.designName;
                cout << defDB.designName << endl;
                semicolonCheck(defStream);
                break;
            case UNITS:
                defStream >> token1 >> token2;
                assert(token1 == "DISTANCE");
                assert(token2 == "MICRONS"); 
                defStream >> defDB.dbuPerMicro;
                cout << defDB.dbuPerMicro << endl;
                semicolonCheck(defStream);
                break;

            case DIEAREA:
                defStream >> token1;
                assert(token1 == "(");
                defStream >> defDB.dieArea.lowerLeft.x >> defDB.dieArea.lowerLeft.y;
                defStream >> token2 >> token1;
                assert(token2 == ")");
                assert(token1 == "(");
                defStream >> defDB.dieArea.upperRight.x >> defDB.dieArea.upperRight.y;
                defStream >> token2;
                assert(token2 == ")");
                semicolonCheck(defStream);

                cout << defDB.dieArea.lowerLeft.x << " " << defDB.dieArea.lowerLeft.y << endl;
                cout << defDB.dieArea.upperRight.x << " " <<  defDB.dieArea.upperRight.y << endl;       
                break;
            case ROW:
                defDB.parseRow(defStream);
                break;
            case TRACKS:
                defDB.parseTracks(defStream);
                break;
            case COMPONENTS:
                defDB.parseComponents(defStream);
                break;
            case PINS:
                defDB.parseIOPins(defStream);
                break;
            case SPECIALNETS:
                defDB.parseSpecialNets(defStream);
                break;
            case NETS:
                defDB.parseNets(defStream);
                break;
            case VIAS:
                defDB.parsedefVias(defStream);
                break;
            case GCELLGRID:
                defDB.parseGcellGrid(defStream);
                semicolonCheck(defStream);
                break;
            case END:
                defStream >> token;
                if(token == "DESIGN")
                {
                    cout << "End of design" << endl;
                    endOfDesign = true;
                    break;
                }
                else {
                    cout << "unknown token : " << token << endl;
                    exit(1);
                }
            default:
                cout << "Error: unsupported keyword " << token << endl;
                exit(1);
        }
        cnt++;
        if(endOfDesign)
            break;
    }
}*/


/*void readLef(string lefFileName)
{
    //string filename("ispd18_test1.input.lef");

    ifstream infile(lefFileName);
    if(!infile.is_open()) {
        cout << "Unable to open file: " << lefFileName << endl;
        exit(1);
    }
   
    std::stringstream lefStream;
    lefStream = removeComments(infile);

    int cnt = 0;
    string token, token1, token2;
    string comment;
    string strOBS;
    bool endOfLibrary = false;
    while(!lefStream.eof())
    {
        lefStream >> token;
        //   cout << token << endl;
        lefKeyword keyword = token2lefKeyword(token);
        
        switch(keyword) {
            case lefVERSION: 
                lefStream >> lefDB.version;
                cout << lefDB.version << endl;
                semicolonCheck(lefStream);
                break;
            case lefDIVIDERCHAR:
                lefStream >> lefDB.dividerChar;
                cout << lefDB.dividerChar << endl;
                semicolonCheck(lefStream);
                break;
            case lefBUSBITCHARS:
                lefStream >> lefDB.busBitChars;
                cout << lefDB.busBitChars << endl;
                semicolonCheck(lefStream);
                break;
            case lefMANUFACTURINGGRID:
                lefStream >> lefDB.manufacturingGrid;
                cout << lefDB.manufacturingGrid << endl;
                semicolonCheck(lefStream);
                break;
            case lefUNITS:
                lefDB.parseUnits(lefStream);
                break;
            case lefPROPERTYDEFINITIONS:
                lefDB.parsePROPERTYDEFINITIONS(lefStream);
                break;

            case lefLAYER:
                lefDB.parseLayer(lefStream);
                break;
            case lefMACRO:
                lefDB.parseMacro(lefStream);
                break;
            case lefVIA:
                lefDB.parselefVia(lefStream);
                break;
            
            case lefSITE:
                lefDB.parseSite(lefStream);
                break;

            case lefCLEARANCEMEASURE:
                lefStream >> lefDB.clearanceMeasure;
                semicolonCheck(lefStream);
                break;

            case lefUSEMINSPACING:
                lefStream >> strOBS >> lefDB.useMinSpacing;
                assert(strOBS == "OBS");
                semicolonCheck(lefStream);
                break;
            case lefVIARULE:
                lefDB.parseViaRule(lefStream);
                break;
                
            case lefEND:
                lefStream >> token;
                if(token == "LIBRARY")
                {
                    cout << "End of library" << endl;
                    endOfLibrary = true;
                    break;
                }
                else {
                    cout << "unknown token : " << token << endl;
                    exit(1);
                }
            default:
                cout << "Error: unsupported keyword " << token << endl;
                exit(1);
        }
        cnt++;
        if(endOfLibrary)
            break;
    }

}*/

#ifndef OPENDB

void readDef(string defFileName) {
    FILE* f;
    int res;

    defrInit();
    defrReset();

    defrInitSession(1);

    defrSetUserData((defiUserData)&defDB);

    defrSetVersionCbk(getDefVersion);
    defrSetBusBitCbk(getDefBusBit);
    defrSetDividerCbk(getDefDivider);

    defrSetDesignCbk(getDefString);
    defrSetDesignEndCbk(getDefVoid);
    defrSetDieAreaCbk(getDefDieArea);
    defrSetUnitsCbk(getDefUnits);
    defrSetRowCbk(getDefRow);
    defrSetTrackCbk(getDefTracks);
    defrSetComponentCbk(getDefComponents);
    defrSetPinCbk(getDefIOPins);
    
    defrSetSNetCbk(getDefSNets);
    defrSetAddPathToNet();
    defrSetNetCbk(getDefNets);
    
    defrSetViaCbk(getDefVias);
    defrSetGcellGridCbk(getDefGcell);


    if ((f = fopen(defFileName.c_str(),"r")) == 0) {
        cout <<"Couldn't open def file" <<endl;
        exit(2);
    }

    res = defrRead(f, defFileName.c_str(), (defiUserData)&defDB, 1);
    if (res != 0) {
        cout <<"DEF parser returns an error!" <<endl;
        exit(2);
    }
    fclose(f);

    //numPins = readPinCnt;

    defrClear();
}


void writeDef(string defFileName) {
    FILE* f;
    int res;


    /*defrSetDesignCbk(getDefString);
    defrSetDesignEndCbk(getDefVoid);
    defrSetDieAreaCbk(getDefDieArea);
    defrSetUnitsCbk(getDefUnits);
    defrSetTrackCbk(getDefTracks);
    defrSetComponentCbk(getDefComponents);
    defrSetPinCbk(getDefIOPins);
    
    defrSetSNetCbk(getDefSNets);
    defrSetAddPathToNet();
    defrSetNetCbk(getDefNets);
    
    defrSetViaCbk(getDefVias);
    defrSetGcellGridCbk(getDefGcell);*/


    if ((f = fopen(defFileName.c_str(),"w")) == 0) {
        cout <<"Couldn't open write def file" <<endl;
        exit(2);
    }

    int status = defwInitCbk(f);

    // set the callback functions
    //defwSetArrayCbk (arrayCB);
    defwSetVersionCbk (writeVersion);
    defwSetBusBitCbk (writeBusBit);
    defwSetDividerCbk (writeDivider);

    defwSetDesignCbk (writeDesignName);
    defwSetDesignEndCbk (writeDesignEnd);
    
    
    defwSetUnitsCbk (writeUnits);
    defwSetDieAreaCbk (writeDieArea);
    defwSetRowCbk (writeRows);
    defwSetTrackCbk (writeTracks);
    defwSetComponentCbk (writeComponents);
    defwSetPinCbk (writeIOPins);
    defwSetNetCbk (writeNets);
    /*defwSetDesignCbk (designCB);
    defwSetDesignEndCbk ((defwVoidCbkFnType)designendCB);
    defwSetDieAreaCbk (dieareaCB);
    defwSetExtCbk (extensionCB);
    defwSetFloorPlanCbk (floorplanCB);
    defwSetGcellGridCbk (gcellgridCB);
    defwSetGroupCbk (groupCB);
    defwSetHistoryCbk (historyCB);
    defwSetPinPropCbk (pinpropCB);
    defwSetPropDefCbk (propdefCB);
    defwSetRegionCbk (regionCB);
    defwSetSNetCbk (snetCB);
    defwSetTechnologyCbk (technologyCB);
    defwSetTrackCbk (trackCB);
    defwSetUnitsCbk (unitsCB);
    defwSetViaCbk (viaCB);*/


    defwSetSNetCbk (clusterWriteSNet);

    res = defwWrite(f, defFileName.c_str(), (void*)&defDB);

    fclose(f);
}

void readLef(string lefFileName)
{
    FILE* f;
    int res;

    lefrInitSession(1);

    lefrSetUserData ((lefiUserData)&lefDB);

    lefrSetMacroCbk(getLefMacros);
    lefrSetMacroBeginCbk(getLefString);
    lefrSetMacroEndCbk(getLefString);
    lefrSetUnitsCbk(getLefUnits);
    lefrSetManufacturingCbk(getLefManufacturingGrid);
    lefrSetPinCbk(getLefPins);
    lefrSetObstructionCbk(getLefObs);
    lefrSetLayerCbk(getLefLayers);
    lefrSetViaCbk(getLefVias);
    lefrSetViaRuleCbk(getLefViaGenerateRules);

    if ((f = fopen(lefFileName.c_str(),"r")) == 0) {
        cout <<"Couldn't open lef file" <<endl;
        exit(2);
    }

    res = lefrRead(f, lefFileName.c_str(), (lefiUserData)&lefDB);
    if (res != 0) {
        cout <<"LEF parser returns an error!" <<endl;
        exit(2);
    }
    fclose(f);

    lefrClear();
}
#else
using namespace odb;
odb::dbLib*
odb_read_lef(odb::dbDatabase* db, const char* path)
{
    if (db == NULL) {
        db = dbDatabase::create();
    }
    lefin lefParser(db, false);
    const char *libname = basename(const_cast<char*>(path));
    if (!db->getTech()) {
        return lefParser.createTechAndLib(libname, path);
    } else {
        return lefParser.createLib(libname, path);
    }
}

std::vector<odb::dbLib*>
odb_read_lef(odb::dbDatabase* db, std::vector<std::string> paths)
{
    if (db == NULL) {
        db = dbDatabase::create();
    }
    std::vector<odb::dbLib*> libs;

    for (std::string &path: paths) {
        libs.push_back(odb_read_lef(db, path.c_str()));
    }
    return libs;
}

odb::dbChip*
odb_read_def(std::vector<odb::dbLib*>& libs, std::vector<std::string> paths)
{
    if (paths.size() != 1) {
        fprintf(stderr, "Only one DEF file should be provided.\n");
        return NULL;
    }
    if (!libs.size()) {
        fprintf(stderr, "No library file(s) provided.\n");
        return NULL;
    }
    dbDatabase *db = libs[0]->getDb();
    defin defParser(db);
    return defParser.createChip(libs, paths[0].c_str());
}

odb::dbChip*
odb_read_def(odb::dbDatabase* db, std::vector<std::string> paths)
{
    std::vector<odb::dbLib *> libs;
    for (auto it = db->getLibs().begin(); it != db->getLibs().end(); it++) {
        libs.push_back(*it);
    }
    return odb_read_def(libs, paths);
}

void readDef(dbDatabase* db)
{
    dbChip* chip = db->getChip();
    dbBlock* chipBlock = chip->getBlock();
    dbTech* tech = db->getTech();

    dbDefDieArea(chipBlock);
    
    dbDefUnits(tech);
    dbDefTracks(chipBlock);
   
    dbDefGcellGrids(chipBlock);
    
    dbDefComponents(chipBlock);


    /*defrSetDesignCbk(getDefString);
    defrSetDesignEndCbk(getDefVoid);
    defrSetDieAreaCbk(getDefDieArea);
    defrSetUnitsCbk(getDefUnits);
    defrSetTrackCbk(getDefTracks);
    defrSetComponentCbk(getDefComponents);
    defrSetPinCbk(getDefIOPins);
    
    defrSetSNetCbk(getDefSNets);
    defrSetAddPathToNet();
    defrSetNetCbk(getDefNets);
    
    defrSetViaCbk(getDefVias);
    defrSetGcellGridCbk(getDefGcell);*/
    
}

void readLef(dbDatabase* db)
{
    //dbLib& lib = db->getLibs().at(0);
    dbTech* tech = db->getTech();
    dbLib* lib = *(db->getLibs().begin());

    //cout << "lefDBU: " << tech->getDbUnitsPerMicron() << endl;
    lefDB.dbuPerMicron = tech->getDbUnitsPerMicron();

    dbLefMacros(lib);
    dbLefLayers(tech);
    
    dbLefVias(tech);
    dbLefViaRules(tech);
    dbLefViaGenerateRules(tech);

    /*lefrSetMacroCbk(getLefMacros);
    lefrSetMacroBeginCbk(getLefString);
    lefrSetMacroEndCbk(getLefString);
    lefrSetUnitsCbk(getLefUnits);
    lefrSetManufacturingCbk(getLefManufacturingGrid);
    lefrSetPinCbk(getLefPins);
    lefrSetObstructionCbk(getLefObs);
    lefrSetLayerCbk(getLefLayers);
    lefrSetViaCbk(getLefVias);
    lefrSetViaRuleCbk(getLefViaRules);*/
}




#endif

void updateDefGcellGrid(int xtrack_step, int ytrack_step)
{
    parser::GcellGrid tmpGcellGrid_x, tmpGcellGrid_x1, tmpGcellGrid_y, tmpGcellGrid_y1;
    int xgcell_step = xtrack_step * 15;
    int ygcell_step = ytrack_step * 15;
    tmpGcellGrid_x.start = defDB.dieArea.lowerLeft.x;
    tmpGcellGrid_x.numBoundaries = (defDB.dieArea.upperRight.x - defDB.dieArea.lowerLeft.x) / xgcell_step + 1;
    tmpGcellGrid_x.step = xgcell_step;
    tmpGcellGrid_x.direction = "X" ;

    tmpGcellGrid_y.start = defDB.dieArea.lowerLeft.y;
    tmpGcellGrid_y.numBoundaries = (defDB.dieArea.upperRight.y - defDB.dieArea.lowerLeft.y) / ygcell_step + 1;
    tmpGcellGrid_y.step = ygcell_step;
    tmpGcellGrid_y.direction = "Y" ;

    tmpGcellGrid_x1.start = tmpGcellGrid_x.start + tmpGcellGrid_x.step * (tmpGcellGrid_x.numBoundaries - 1);
    tmpGcellGrid_x1.numBoundaries = 2;
    tmpGcellGrid_x1.step = defDB.dieArea.upperRight.x - tmpGcellGrid_x1.start;
    tmpGcellGrid_x1.direction = "X" ;

    tmpGcellGrid_y1.start = tmpGcellGrid_y.start + tmpGcellGrid_y.step * (tmpGcellGrid_y.numBoundaries - 1);
    tmpGcellGrid_y1.numBoundaries = 2;
    tmpGcellGrid_y1.step = defDB.dieArea.upperRight.y - tmpGcellGrid_y1.start;
    tmpGcellGrid_y1.direction = "Y" ;

    cout << "GCELLGRID TO BE ADDED: " << endl;
    cout << "GCELLGRID X " << tmpGcellGrid_x.start << " DO " << tmpGcellGrid_x.numBoundaries << " STEP " << tmpGcellGrid_x.step << endl;
    cout << "GCELLGRID X " << tmpGcellGrid_x1.start << " DO " << tmpGcellGrid_x1.numBoundaries << " STEP " << tmpGcellGrid_x1.step << endl;
    cout << "GCELLGRID Y " << tmpGcellGrid_y.start << " DO " << tmpGcellGrid_y.numBoundaries << " STEP " << tmpGcellGrid_y.step << endl;
    cout << "GCELLGRID Y " << tmpGcellGrid_y1.start << " DO " << tmpGcellGrid_y1.numBoundaries << " STEP " << tmpGcellGrid_y1.step << endl;

    defDB.gcellGrids.push_back(tmpGcellGrid_x);
    defDB.gcellGrids.push_back(tmpGcellGrid_x1);

    defDB.gcellGrids.push_back(tmpGcellGrid_y);
    defDB.gcellGrids.push_back(tmpGcellGrid_y1);
};

void generateGcellGrid()
{
    int xtrack_step = 0, ytrack_step = 0;
    string routing_layer_name;
    for(auto layer : lefDB.layers)
    {
        if(layer.type == "ROUTING")
        {
            cout << "layer for gcell: " << layer.name << endl;
            routing_layer_name = layer.name;
            for(auto track : defDB.tracks)
            {
                for(auto layerName : track.layerNames)
                {
                    if(layerName == layer.name && track.direction == "Y")
                        ytrack_step = track.step;

                    if(layerName == layer.name && track.direction == "X")
                        xtrack_step = track.step;
                }
            }
            break;
        }
    }
    cout << "for GcellGrid xtrack_step: " << xtrack_step << "ytrack_step: " << ytrack_step << endl;

    updateDefGcellGrid(xtrack_step, ytrack_step);

}

void initGcell()
{
    bool enableOutput = false;

    std::list<int> tmpXGcellBoundaries, tmpYGcellBoundaries;
    Point2D<int> maxBoundaryCnt;
    maxBoundaryCnt.x = 0;
    maxBoundaryCnt.y = 0;
    if(defDB.gcellGrids.size() == 0)
        generateGcellGrid();
    
    for(auto gcellGrid : defDB.gcellGrids)
    {
        if(gcellGrid.direction == "X")
        {   
            if(gcellGrid.numBoundaries > maxBoundaryCnt.x)
            {
                maxBoundaryCnt.x = gcellGrid.numBoundaries;
                defDB.commonGcell.x = gcellGrid.step;
            }

            for(int i = 0; i < gcellGrid.numBoundaries; i++)
            {
                if(std::find(tmpXGcellBoundaries.begin(), tmpXGcellBoundaries.end(), gcellGrid.start + i*gcellGrid.step) == tmpXGcellBoundaries.end())
                    tmpXGcellBoundaries.push_back(gcellGrid.start + i*gcellGrid.step);
            }
        }
        else if(gcellGrid.direction == "Y")
        {
            if(gcellGrid.numBoundaries > maxBoundaryCnt.y)
            {
                maxBoundaryCnt.y = gcellGrid.numBoundaries;
                defDB.commonGcell.y = gcellGrid.step;
            }
            for(int i = 0; i < gcellGrid.numBoundaries; i++)
            {
                if(std::find(tmpYGcellBoundaries.begin(), tmpYGcellBoundaries.end(), gcellGrid.start + i*gcellGrid.step) == tmpYGcellBoundaries.end())
                    tmpYGcellBoundaries.push_back(gcellGrid.start + i*gcellGrid.step);
            }
        }
        else
        {
            cout << "unknown gcell direction: " << gcellGrid.direction << endl;
            exit(1);
        }
    }
    tmpXGcellBoundaries.sort();
    tmpYGcellBoundaries.sort();

    defDB.xGcellBoundaries.clear();
    defDB.yGcellBoundaries.clear();

    for(auto xBoundary : tmpXGcellBoundaries)
        defDB.xGcellBoundaries.push_back(xBoundary);

    for(auto yBoundary : tmpYGcellBoundaries)
        defDB.yGcellBoundaries.push_back(yBoundary);

    if(enableOutput)
    {
        cout << "xboundaries: " <<endl;
        int print_cnt = 0;
        for(auto boundary : defDB.xGcellBoundaries)
        {
            cout << boundary << " ";
            if(print_cnt % 10 == 0)
                cout << endl;
            print_cnt++;
        }
        cout << endl;
    
        cout << "yboundaries: " <<endl;
        print_cnt = 0;
        for(auto boundary : defDB.yGcellBoundaries)
        {
            cout << boundary << " ";
            if(print_cnt % 10 == 0)
                cout << endl;
            print_cnt++;
        }
        cout << endl;
    }

    
    int numRoutingLayers = 0;
    for(auto layer : lefDB.layers)
    {
        if(layer.type == "ROUTING")
        {
            numRoutingLayers++;
        }
    }

    defDB.gcellGridDim.x = defDB.xGcellBoundaries.size() - 1;
    defDB.gcellGridDim.y = defDB.yGcellBoundaries.size() - 1;
    defDB.gcellGridDim.z = numRoutingLayers;
    
    assert(defDB.gcellGridDim.x > 0 && defDB.gcellGridDim.y > 0 && defDB.gcellGridDim.z > 0);
    //cout << "here? " << defDB.gcellGridDim.x << " " <<  defDB.gcellGridDim.y << " " <<  defDB.gcellGridDim.z << endl;
    defDB.gcells.resize(defDB.gcellGridDim.x * defDB.gcellGridDim.y * defDB.gcellGridDim.z);

    
}

template <typename T>
Range<T> rangeIntersection(Range<T> r1, Range<T> r2)
{
    Range<T> tmp;
    tmp.start = (r1.start > r2.start)? r1.start : r2.start;
    tmp.end = (r1.end < r2.end)? r1.end : r2.end;
    if(tmp.start > tmp.end)
        return Range<T>(0, 0);
    else
        return tmp;
}

void GcellInsertOBS(int x, int y, int z, Rect2D<int> rect, int OBSIdx)
{
    Gcell& gcell = defDB.getGcell(x, y, z);
    string layerName = lefDB.layers.at(z*2).name;
    string direction = lefDB.layers.at(z*2).direction;

    if(defDB.layerName2trackidx.count(layerName) == 0)
    {
        cout << "unable to find layer tracks!: " << layerName << endl;
        exit(1);
    }
    

    int gcellLower, gcellUpper;
    if(direction == "HORIZONTAL")// HORIZONTAL LAYER
    {
        gcellLower = defDB.yGcellBoundaries.at(y);
        gcellUpper = defDB.yGcellBoundaries.at(y + 1);
    }
    else if(direction == "VERTICAL")
    {
        gcellLower = defDB.xGcellBoundaries.at(x);
        gcellUpper = defDB.xGcellBoundaries.at(x + 1);
    }
    else
    {
        cout << "unkown direction of layer : " << layerName << endl;
        exit(1);
    }

    int trackIdx = defDB.layerName2trackidx.find(layerName)->second;
    Track track = defDB.tracks.at(trackIdx);
    int trackStart = track.start;
    int trackStep = track.step;

    int gcellTrackStart = (gcellLower - trackStart) / trackStep + 1; // both are inclusive
    int gcellTrackEnd = (gcellUpper - trackStart) / trackStep;

    Range<int> gcellRange(gcellTrackStart, gcellTrackEnd);

    /*if(1)
    {   
        rect.print();
        cout << "layer:" << layerName << " overlap (" << gcellLower << ")gcelltrackstart " << gcellTrackStart; 
        cout << " (" << gcellUpper << ")gcellTrackEnd: " << gcellTrackEnd << " start " << trackStart << " step" << trackStep << endl; 
    }*/

    if(gcell.trackUsed == NULL)
    {
        gcell.numTracks = gcellTrackEnd - gcellTrackStart + 1;
        gcell.trackUsed = new bool [gcell.numTracks];
        for(int i = 0; i < gcell.numTracks; i++)
        {
            gcell.trackUsed[i] = false;
        }

    }

    /*rect.lowerLeft.x -= trackStep;
    rect.lowerLeft.y -= trackStep;
    rect.upperRight.x += trackStep;
    rect.upperRight.y += trackStep;*/

    Range<int> OBSTrackRange = getTrackRange(z*2, rect, true);

    Range<int> overlapRange = rangeIntersection(gcellRange, OBSTrackRange);
    int localStart = overlapRange.start - gcellTrackStart;
    int localEnd = overlapRange.end - gcellTrackStart;

    /*if(x == 16 && y == 17)
    {
        cout << rect << endl;
        cout << "OBS " << OBSTrackRange.start << " " << OBSTrackRange.end << endl;
        cout << "Gcell " << gcellRange.start << " " << gcellRange.end << endl; 
        cout << "Overlap " << localStart << " " << localEnd << endl;
    }
    cout << "overlap range:" << localStart << " " << localEnd << endl;*/

    for(int i = localStart; i <= localEnd; i++)
    {
        gcell.trackUsed[i] = true;
    }

    gcell.OBSIDlist.push_back(OBSIdx);

    Point2D<int> GcellLowerLeft(defDB.xGcellBoundaries.at(x), defDB.yGcellBoundaries.at(y));
    Point2D<int> GcellUpperRight(defDB.xGcellBoundaries.at(x + 1), defDB.yGcellBoundaries.at(y + 1));

    if(rect.lowerLeft.x <= GcellLowerLeft.x && rect.lowerLeft.y <= GcellLowerLeft.y && rect.upperRight.x >= GcellUpperRight.x && rect.upperRight.y >= GcellUpperRight.y)
        gcell.obs_state = FULLYCOVERED;
    else
    {
        gcell.obs_state = HAS_OBS;
    }

}

void preprocessDesignRuleOBS()
{
    for(int i = 0; i <  lefDB.layers.size(); i++)
    {
        if(defDB.designRuleOBS.at(i).size() == 0 || lefDB.layers.at(i).type != "ROUTING")
            continue;
        string layerName = lefDB.layers.at(i).name;

        cout << layerName << endl;

        int trackIdx = defDB.layerName2trackidx.find(layerName)->second;
        int trackStep = defDB.tracks.at(trackIdx).step;

        for(int OBSIdx = 0; OBSIdx < defDB.designRuleOBS.at(i).size(); OBSIdx++)
        {
            auto rect = defDB.designRuleOBS.at(i).at(OBSIdx);

            //if(rect.lowerLeft.x == 1793920 && rect.lowerLeft.y == 1560150)
            //    cout << "ok i find this obs" << endl;

            int xGcell_start = 0;
            int yGcell_start = 0;
            int xGcell_end = 0;
            int yGcell_end = 0;

            xGcell_start = find_Gcell(rect.lowerLeft.x - trackStep, defDB.xGcellBoundaries);
            yGcell_start = find_Gcell(rect.lowerLeft.y - trackStep, defDB.yGcellBoundaries);
            xGcell_end = find_Gcell(rect.upperRight.x + trackStep, defDB.xGcellBoundaries);
            yGcell_end = find_Gcell(rect.upperRight.y + trackStep, defDB.yGcellBoundaries);

            //cout << xGcell_start << " " << yGcell_start << " " << xGcell_end << " " << yGcell_end << endl;

            //cout << " ========== " << endl;
            if(!MAX_FLOW)
            {
                if(lefDB.layers.at(i).direction == "HORIZONTAL" && yGcell_end < defDB.gcellGridDim.y - 1)
                    yGcell_end += 1;
                else if(lefDB.layers.at(i).direction == "VERTICAL" && xGcell_end < defDB.gcellGridDim.x - 1)
                    xGcell_end += 1;
                else if(lefDB.layers.at(i).direction != "HORIZONTAL" && lefDB.layers.at(i).direction != "VERTICAL")
                {
                    cout << "unknown direction!" << endl;
                    exit(1);
                }
            }
            else
            {
                if(yGcell_end < defDB.gcellGridDim.y - 1)
                    yGcell_end += 1;
                if(xGcell_end < defDB.gcellGridDim.x - 1)
                    xGcell_end += 1;
            }
            for(int x = xGcell_start; x < xGcell_end; x++)
            {
                for(int y = yGcell_start; y < yGcell_end; y++)
                {
                    /*CapReduction tmpCapReduction;
                    tmpCapReduction.x = x;
                    tmpCapReduction.y = y;
                    tmpCapReduction.z = i / 2;*/
                    //cout << " OBS GRID: " << x << " " << y << endl;
                    GcellInsertOBS(x, y , i / 2, rect, OBSIdx); 
                    /*CapReduction tmpCapReduction;
                    tmpCapReduction.x = x;
                    tmpCapReduction.y = y;
                    tmpCapReduction.z = i / 2;
                    tmpCapReduction.newCap = 0;
                    defDB.capReductions.push_back(tmpCapReduction);*/
                }
                
            }
            
        }
    }
    
}

int maxFlowGcell(int x, int y, int z, GraphTemplate* graphTemplates)
{
    int next_x, next_y;
    int gcellBoundary;
    Gcell gcell = defDB.getGcell(x, y, z);
    if(lefDB.layers.at(z * 2).direction == "HORIZONTAL")
    {
        next_x = x + 1;
        next_y = y;
        gcellBoundary = defDB.xGcellBoundaries.at(x + 1);
    }
    else if(lefDB.layers.at(z * 2).direction == "VERTICAL")
    {
        next_x = x;
        next_y = y + 1;
        gcellBoundary = defDB.yGcellBoundaries.at(y + 1);
    }
    else{
        cout << "unknown layer direction: " << lefDB.layers.at(z * 2).name << endl;
        exit(1);
    }

    Gcell nextGcell = defDB.getGcell(next_x, next_y, z);

    if(gcell.obs_state == FULLYCOVERED || nextGcell.obs_state == FULLYCOVERED)
        return 0;

    Point2D<int> GcellLowerLeft(defDB.xGcellBoundaries.at(x), defDB.yGcellBoundaries.at(y));
    Point2D<int> GcellUpperRight(defDB.xGcellBoundaries.at(next_x), defDB.yGcellBoundaries.at(next_y));
    Rect2D<int> GcellRect(GcellLowerLeft, GcellUpperRight);

    std::set<int> curOBSIDlist, botOBSIDlist, topOBSIDlist;
    std::vector<Rect2D<int>> curOBSlist, botOBSlist, topOBSlist;
//===============================================================
    for(auto OBSIdx : gcell.OBSIDlist)
    {
        curOBSIDlist.insert(OBSIdx);
    }    
    for(auto OBSIdx : nextGcell.OBSIDlist)
    {
        curOBSIDlist.insert(OBSIdx);
    }
    for(auto OBSIdx : curOBSIDlist)
    {
        auto rect = defDB.designRuleOBS.at(z * 2).at(OBSIdx);
        curOBSlist.push_back(rect);
    }

//================================================================
    //add track offset of bot layer

    if(z - 1 > 0)
    {
        Gcell botGcell = defDB.getGcell(x, y, z - 1);
        Gcell nextBotGcell = defDB.getGcell(next_x, next_y, z - 1);
        for(auto OBSIdx : botGcell.OBSIDlist)
        {
            botOBSIDlist.insert(OBSIdx);
        }    
        for(auto OBSIdx : nextBotGcell.OBSIDlist)
        {
            botOBSIDlist.insert(OBSIdx);
        }
        for(auto OBSIdx : botOBSIDlist)
        {
            auto rect = defDB.designRuleOBS.at((z - 1) * 2).at(OBSIdx);
            botOBSlist.push_back(rect);
        }
    }
//================================================================
    //add track offset of top layer

    if(z + 1 < defDB.gcellGridDim.z)
    {
        Gcell topGcell = defDB.getGcell(x, y, z + 1);
        Gcell nextTopGcell = defDB.getGcell(next_x, next_y, z + 1);
        for(auto OBSIdx : topGcell.OBSIDlist)
        {
            topOBSIDlist.insert(OBSIdx);
        }    
        for(auto OBSIdx : nextTopGcell.OBSIDlist)
        {
            topOBSIDlist.insert(OBSIdx);
        }
        for(auto OBSIdx : topOBSIDlist)
        {
            auto rect = defDB.designRuleOBS.at((z + 1) * 2).at(OBSIdx);
            topOBSlist.push_back(rect);
        }
    }
    int x_offset = x % 2;

    int y_offset = y % 2;
    GraphTemplate& graphTemplate = graphTemplates[z * 2 * 2 + y_offset * 2 + x_offset];

    //genFlowGraph
    FlowGraph graph;

    constructGraph(graph, graphTemplate, z, GcellRect, curOBSlist, botOBSlist, topOBSlist);

    int flow = computeMaxFlow(graph, graphTemplate.edgelist.size());

    return flow;
}

void adjustGcellCap(GraphTemplate* graphTemplates, galois::InsertBag<CapReduction>& capReductions)
{
    std::vector<WorkingGcell> workingGcells;
    for(int i = 1; i <  lefDB.layers.size(); i++)
    {
        if(defDB.designRuleOBS.at(i).size() != 0 && lefDB.layers.at(i).type == "ROUTING")
        {
            cout << lefDB.layers.at(i).name << endl;
        
            for(int x = 0; x < defDB.xGcellBoundaries.size() - 2; x++)
            {
                //cout << "flow graph: " << x << endl;
                for(int y = 0; y < defDB.yGcellBoundaries.size() - 2; y++)
                {
                    //cout << "flow graph: " << x << " " << y << endl;
                    Gcell& gcell = defDB.getGcell(x, y, i/2);
                    if(MAX_FLOW)
                    {
                        int next_x, next_y;
                        if(lefDB.layers.at(i).direction == "HORIZONTAL")
                        {
                            next_x = x + 1;
                            next_y = y;
                        }
                        else if(lefDB.layers.at(i).direction == "VERTICAL")
                        {
                            next_x = x;
                            next_y = y + 1;
                        }
                        else{
                            cout << "unknown layer direction: " << lefDB.layers.at(i).name << endl;
                            exit(1);
                        }
                        Gcell& nextGcell = defDB.getGcell(next_x, next_y, i/2);

                        if(gcell.obs_state == NO_OBS && nextGcell.obs_state == NO_OBS)
                            continue;

                        if(next_x > defDB.gcellGridDim.x || next_y > defDB.gcellGridDim.y)
                            continue;

                        CapReduction tmpCapReduction;
                        tmpCapReduction.x = x;
                        tmpCapReduction.y = y;
                        tmpCapReduction.z = i / 2;
                        if(gcell.obs_state == FULLYCOVERED || nextGcell.obs_state == FULLYCOVERED)
                            tmpCapReduction.newCap = 0;
                        else 
                        {
                            workingGcells.push_back(WorkingGcell(x, y, i/2));
                            continue;
                        }
                            //tmpCapReduction.newCap = maxFlowGcell(x, y, i/2, graphTemplates);
                        assert(tmpCapReduction.newCap >= 0);

                        //if(tmpCapReduction.newCap != 0)
                        //cout << x << " " << y << " flow: " << tmpCapReduction.newCap << endl;

                        capReductions.push(tmpCapReduction);

                    }
                    else
                    {
                        int numUsedTrack = 0;
                        for(int trackIdx = 0; trackIdx < gcell.numTracks; trackIdx++)
                        {
                            if(gcell.trackUsed[trackIdx])
                                numUsedTrack++;
                        }

                        if(gcell.numTracks == 0)
                            continue;


                        CapReduction tmpCapReduction;
                        tmpCapReduction.x = x;
                        tmpCapReduction.y = y;
                        tmpCapReduction.z = i / 2;
                        tmpCapReduction.newCap = gcell.numTracks - numUsedTrack;
                        assert(tmpCapReduction.newCap >= 0);

                        capReductions.push(tmpCapReduction);
                    }
                }
            }
        }
    }

    if(MAX_FLOW)
    {

        cout << " galois parallel: " << endl;
        galois::on_each( 
                [&] (const unsigned tid, const unsigned numT)
                {
                    printf("threadid: %d %d\n", tid, numT);
                }
                );
        cout << "maxflow problem cnt: " << workingGcells.size() << endl;
        //exit(1);
        galois::do_all(galois::iterate(workingGcells),
            [&] (const WorkingGcell& gcell) {
                //if(gcell.z <= 5 || gcell.x <= 8 || gcell.y <= 90)
                //    return;
                //cout << gcell.x << " " << gcell.y << " " << gcell.z;
                CapReduction tmpCapReduction;
                tmpCapReduction.x = gcell.x;
                tmpCapReduction.y = gcell.y;
                tmpCapReduction.z = gcell.z;
                tmpCapReduction.newCap = maxFlowGcell(gcell.x, gcell.y, gcell.z, graphTemplates);
                assert(tmpCapReduction.newCap >= 0);

                capReductions.push(tmpCapReduction);
                //cout << " cap: " << tmpCapReduction.newCap << endl;
            },
            galois::loopname("compute max flow"),
            galois::steal()
            );
    }
    
}

void preprocessSNet()
{
    int dbuPerMicro = defDB.dbuPerMicro;
    for(auto snet : defDB.snets)
    {
        for(auto path : snet.paths)
        {
            if(path.viaName == "")
            {
                int layerIdx = lefDB.layer2idx.find(path.layerName)->second;
                if(layerIdx > 0)
                {
                    if(!path.rect.empty())
                    {
                        Rect2D<float> tmpRect;
                        tmpRect.set(path.rect.lowerLeft.x, path.rect.lowerLeft.y, path.rect.upperRight.x, path.rect.upperRight.y);
                        defDB.origOBS.at(layerIdx).push_back(tmpRect);
                    }
                    else
                    {
                        Rect2D<float> tmpRect;
                        int llx, lly, urx, ury;

                        if(path.begin.empty() || path.end.empty())
                        {
                            cout << " path missing begin or end node!" << endl;
                            path.begin.print();
                            path.end.print();
                            exit(1); 
                        }
                        
                        if(path.begin.y  == path.end.y)
                        {
                            llx = (path.begin.x < path.end.x)? path.begin.x : path.end.x;
                            urx = (path.begin.x > path.end.x)? path.begin.x : path.end.x;
                            lly = path.begin.y;
                            ury = path.begin.y;
                        }
                        else if(path.begin.x  == path.end.x)
                        {
                            llx = path.begin.x;
                            urx = path.begin.x;
                            lly = (path.begin.y < path.end.y)? path.begin.y : path.end.y;
                            ury = (path.begin.y > path.end.y)? path.begin.y : path.end.y;
                        }
                        else
                        {
                            cout << "ERROR: not a line? " << endl;
                            path.begin.print();
                            path.end.print();
                            exit(1);
                        }
                        llx = llx - path.width/2;
                        lly = lly - path.width/2;
                        urx = urx + path.width/2;
                        ury = ury + path.width/2;
                        /*llx = llx - path.width*2;
                        lly = lly - path.width*2;
                        urx = urx + path.width*2;
                        ury = ury + path.width*2;*/
                        tmpRect.set(llx, lly, urx, ury);

                        defDB.origOBS.at(layerIdx).push_back(tmpRect);

                    }
                }
            }
            else // is a Via
            {
                //if(path.begin.x == 31500 && path.begin.y == 263600)
                //    cout << "here" << endl;
                if(lefDB.lefVia2idx.count(path.viaName))
                {
                    int viaIdx = lefDB.lefVia2idx.find(path.viaName)->second;
                    lefVia via = lefDB.vias.at(viaIdx);
                    for(auto layerRect : via.layerRects)
                    {
                        int layerIdx = lefDB.layer2idx.find(layerRect.layerName)->second;
                        if(layerIdx == 0)
                            continue;
                        auto layer = lefDB.layers.at(layerIdx);
                        if(layer.type == "ROUTING")
                        {
                            for(auto rect : layerRect.rects)
                            {
                                Rect2D<float> tmpRect;
                                float llx = rect.lowerLeft.x * dbuPerMicro;
                                float lly = rect.lowerLeft.y * dbuPerMicro;
                                float urx = rect.upperRight.x * dbuPerMicro;
                                float ury = rect.upperRight.y * dbuPerMicro;
                                tmpRect.set(path.begin.x + llx, path.begin.y + lly, path.begin.x + urx, path.begin.y + ury);
                                defDB.origOBS.at(layerIdx).push_back(tmpRect);
                            }
                        }
                    }
                }
                else if(defDB.defVia2idx.count(path.viaName))
                {
                    int viaIdx = defDB.defVia2idx.find(path.viaName)->second;
                    defVia via = defDB.vias.at(viaIdx);
                    int xshift = ((float)via.numCutCols - 1 ) / 2 * via.cutSpacing.x + via.botEnc.x;
                    int yshift = ((float)via.numCutRows - 1 ) / 2 * via.cutSpacing.y + via.botEnc.y;

                    string botLayer = via.layers[0];
                    string topLayer = via.layers[2];

                    int botLayerIdx = lefDB.layer2idx.find(botLayer)->second;
                    int topLayerIdx = lefDB.layer2idx.find(topLayer)->second;

                    Rect2D<float> tmpRect;
                    tmpRect.set(path.begin.x - xshift, path.begin.y - yshift, path.begin.x + xshift, path.begin.y + yshift);
                    //tmpRect.set(path.begin.x - 2*xshift, path.begin.y - 2*yshift, path.begin.x + 2*xshift, path.begin.y + 2*yshift);

                    defDB.origOBS.at(botLayerIdx).push_back(tmpRect);
                    defDB.origOBS.at(topLayerIdx).push_back(tmpRect);


                }
                else
                {
                    cout << " unable to find via: " << path.viaName << endl;
                }
            }

        }
    }
}



void linkTrackToLayer()
{
    for(auto layer : lefDB.layers)
    {
        if(layer.type == "ROUTING")
        {
            if(layer.direction == "HORIZONTAL")   // HORIZONTAL corresponds to Y location of a track
            {
                for(int i = 0; i < defDB.tracks.size(); i++)
                {
                    auto track = defDB.tracks.at(i);
                    for(auto layerName : track.layerNames)
                    {
                        if(layerName == layer.name && track.direction == "Y") {
                            defDB.layerName2trackidx.insert( pair<string, int> (layer.name, i));
                        }
                    }
                }
            }
            else if(layer.direction == "VERTICAL")
            {
                for(int i = 0; i < defDB.tracks.size(); i++)
                {
                    auto track = defDB.tracks.at(i);
                    for(auto layerName : track.layerNames)
                    {
                        if(layerName == layer.name && track.direction == "X") {
                            defDB.layerName2trackidx.insert( pair<string, int> (layer.name, i));
                        }
                    }
                }
            }
            else {
                cout << "error: unknown direction of lefDB layers" << endl;
                exit(1);
            }
        }
    }
}

void preprocessSpacingTable()
{
    int dbuPerMicro = defDB.dbuPerMicro;
    for(auto& layer:lefDB.layers)
    {   
        //PRL spacing
        auto& spacingTable = layer.spacingTable;
        spacingTable.numCols = spacingTable.parallelRunLength.size();
        spacingTable.numRows = spacingTable.width.size();
        for(auto& length : spacingTable.parallelRunLength)
            length *= dbuPerMicro;

        for(auto& width : spacingTable.width)
            width *= dbuPerMicro;

        spacingTable.setSize(spacingTable.parallelRunLength.size(), spacingTable.width.size());
        for(auto& spacing : spacingTable.spacing)
        {
            spacing *= dbuPerMicro;
        }


        //corner spacing
        auto& cornerSpacing = layer.cornerSpacing;
        cornerSpacing.eolWidth *= dbuPerMicro;
        for(auto& width : cornerSpacing.width)
            width *= dbuPerMicro;

        for(auto& spacing : cornerSpacing.spacing)
            spacing *= dbuPerMicro;


        //EOL spacing
        for(auto& eolSpacing : layer.spacings)
        {
            eolSpacing.spacing *= dbuPerMicro;
            eolSpacing.eolWidth *= dbuPerMicro;
            eolSpacing.eolWithin *= dbuPerMicro;
            eolSpacing.parEdge *= dbuPerMicro;
            eolSpacing.parWithin *= dbuPerMicro;
            eolSpacing.cutWithin *= dbuPerMicro;
        }

        auto& maxEOLSpacing = layer.maxEOLSpacing;

        for(auto& eolSpacing : layer.spacings)
        {
            maxEOLSpacing.spacing = std::max(maxEOLSpacing.spacing, eolSpacing.spacing);
            maxEOLSpacing.eolWidth = std::max(maxEOLSpacing.eolWidth, eolSpacing.eolWidth);
            maxEOLSpacing.eolWithin = std::max(maxEOLSpacing.eolWithin, eolSpacing.eolWithin);
            maxEOLSpacing.parEdge = std::max(maxEOLSpacing.parEdge, eolSpacing.parEdge);
            maxEOLSpacing.parWithin = std::max(maxEOLSpacing.parWithin, eolSpacing.parWithin);
            maxEOLSpacing.cutWithin = std::max(maxEOLSpacing.cutWithin, eolSpacing.cutWithin);
        }
    }
}

void preprocessDesignRule()
{
    for(int i = 0; i <  lefDB.layers.size(); i++)
    {
        if(defDB.origOBS.at(i).size() == 0 || lefDB.layers.at(i).type != "ROUTING")
            continue;
        string layerName = lefDB.layers.at(i).name;

        cout << "design rule: " << layerName << endl;

        int trackIdx = defDB.layerName2trackidx.find(layerName)->second;
        int trackStep = defDB.tracks.at(trackIdx).step;
        Layer& layer = lefDB.layers.at(i);

        for(int OBSIdx = 0; OBSIdx < defDB.origOBS.at(i).size(); OBSIdx++)
        {
            auto rect = defDB.origOBS.at(i).at(OBSIdx);
            
            int x_diff = rect.upperRight.x - rect.lowerLeft.x;
            int y_diff = rect.upperRight.y - rect.lowerLeft.y;
            int length = std::max(x_diff, y_diff);
            int width = std::min(x_diff, y_diff);

            auto lengthIt = std::upper_bound(layer.spacingTable.parallelRunLength.begin(), layer.spacingTable.parallelRunLength.end(), length);
            int col = std::distance(layer.spacingTable.parallelRunLength.begin(), lengthIt) - 1;

            auto widthIt = std::upper_bound(layer.spacingTable.width.begin(), layer.spacingTable.width.end(), width);
            int row = std::distance(layer.spacingTable.width.begin(), widthIt) - 1;

            assert(col >= 0 && col < layer.spacingTable.parallelRunLength.size());
            assert(row >= 0 && row < layer.spacingTable.width.size());

            int PRL_expand = layer.spacingTable.getSpacing(col, row);
            int EOL_expand = 0;
            int COR_expand = 0;

            //consider EOL
            auto& maxEOLSpacing = layer.maxEOLSpacing;
            if(width < maxEOLSpacing.eolWidth)
            {
                EOL_expand = std::max(maxEOLSpacing.spacing, maxEOLSpacing.parEdge);
            }


             //consider corner spacing
            auto& CornerSpacing = layer.cornerSpacing;
            if(width >= CornerSpacing.eolWidth && CornerSpacing.width.size() != 0)
            {
                auto corIt = std::upper_bound(CornerSpacing.width.begin(), CornerSpacing.width.end(), width);
                int row = std::distance(CornerSpacing.width.begin(), corIt) - 1;
                COR_expand = CornerSpacing.spacing.at(row);
            }
            
            int expand = std::max(PRL_expand, EOL_expand);
            expand = std::max(expand, COR_expand);
            Rect2D<int> expandRect;
            expandRect.lowerLeft.x = rect.lowerLeft.x - expand;
            expandRect.lowerLeft.y = rect.lowerLeft.y - expand;
            expandRect.upperRight.x = rect.upperRight.x + expand;
            expandRect.upperRight.y = rect.upperRight.y + expand;
            defDB.designRuleOBS.at(i).push_back(expandRect);

            //cout << "rect: " << rect.lowerLeft.x << "," << rect.lowerLeft.y << " " << rect.upperRight.x << "," << rect.upperRight.y << " expand:" << expand << endl;
        }
    }
}

int main(int argc, char** argv)
{
    string defFileName, lefFileName, outFileName, clusterFileName;
    for(int i = 1; i < argc; i++)
    {
        string tmp(argv[i]);
        if(tmp == "-lef")
        {   
            lefFileName = string(argv[i+1]);
        }
        else if(tmp == "-def")
        {
            defFileName = string(argv[i+1]);
        }
        else if(tmp == "-t")
        {
            numThreads = atoi(argv[i+1]);

        }
        else if(tmp == "-output")
        {   
            outFileName = string(argv[i+1]);
        }
        else if(tmp == "-cluster") {
            clusterFileName = string(argv[i+1]);
        }
    }
    if(defFileName.size() == 0 || defFileName.size() == 0 || outFileName.size() == 0 || clusterFileName.size() == 0)
    {
        cout << "usage: ./lefdef_SPRoute -lef [LefFile] -def [DefFile] -cluster [clusterFile] -t [nthreads] -output [Output file]" << endl;
        exit(1);
    }
    cout << "reading lef: " << lefFileName <<endl;
    cout << "reading def: " << defFileName<<endl;


    if(!lefFileName.empty())
        readLef(lefFileName);
    if(!defFileName.empty())
        readDef(defFileName);


    linkTrackToLayer();
    preprocessSpacingTable();
    
    preprocessComponent(); 

    //simplePWGNDMesh();
    clusterPWGNDMesh(clusterFileName);
    if(!outFileName.empty())
        writeDef(outFileName);
    

    return 1;
}
