/////////////////////////////////////////////////////////////////////////////
// Name:        f.h
// Author:      Laurent Pugin
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_F_H__
#define __VRV_F_H__

#include "atts_shared.h"
#include "textelement.h"
#include "timeinterface.h"

namespace vrv {

//----------------------------------------------------------------------------
// F (figure)
//----------------------------------------------------------------------------

/**
 * This class models the MEI <f> element.
 */
class F : public TextElement, public TimeSpanningInterface, public AttExtender {
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method reset all attribute classes
     */
    ///@{
    F();
    virtual ~F();
    virtual Object *Clone() const { return new F(*this); }
    virtual void Reset();
    virtual std::string GetClassName() const { return "F"; }
    ///@}

    /**
     * @name Getter to interfaces
     */
    ///@{
    virtual TimePointInterface *GetTimePointInterface() { return dynamic_cast<TimePointInterface *>(this); }
    virtual TimeSpanningInterface *GetTimeSpanningInterface() { return dynamic_cast<TimeSpanningInterface *>(this); }
    ///@}

    /**
     * Add an element (text, rend. etc.) to a rend.
     * Only supported elements will be actually added to the child list.
     */
    virtual bool IsSupportedChild(Object *object);

    //----------//
    // Functors //
    //----------//

    // Because F is a TextElement and not a ControlElement, these methods need to be implemented because
    // TextElement does not inherit from FloatingObject.

    /**
     * See Object::FillStaffCurrentTimeSpanning
     */
    int FillStaffCurrentTimeSpanning(FunctorParams *functorParams) override;

    /**
     * See Object::PrepareTimePointing
     */
    int PrepareTimePointing(FunctorParams *functorParams) override;

    /**
     * See Object::PrepareTimeSpanning
     */
    int PrepareTimeSpanning(FunctorParams *functorParams) override;

    /**
     * See Object::PrepareTimestamps
     */
    int PrepareTimestamps(FunctorParams *functorParams) override;

    /**
     * See Object::ResetDrawing
     */
    int ResetDrawing(FunctorParams *functorParams) override;

private:
    //
public:
    //
protected:
private:
};

} // namespace vrv

#endif
