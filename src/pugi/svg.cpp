/////////////////////////////////////////////////////////////////////////////
// Name:        svg.cpp
// Author:      Laurent Pugin
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "fig.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "svg.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Svg
//----------------------------------------------------------------------------

Svg::Svg() : Object("fig-")
{
    Reset();
}

Svg::~Svg()
{
}

void Svg::Reset()
{
    Object::Reset();
}

} // namespace vrv
