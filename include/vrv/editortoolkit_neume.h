/////////////////////////////////////////////////////////////////////////////
// Name:        editortoolkit_neume.h
// Author:      Juliette Regimbal
// Created:     04/06/2019
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_EDITOR_TOOLKIT_NEUME_H__
#define __VRV_EDITOR_TOOLKIT_NEUME_H__

#include <cmath>
#include <string>
#include <utility>

//--------------------------------------------------------------------------------

#include "doc.h"
#include "editortoolkit.h"
#include "view.h"
#include "zone.h"

#include "jsonxx.h"

namespace vrv {

//--------------------------------------------------------------------------------
// EditorToolkitNeume
//--------------------------------------------------------------------------------

class EditorToolkitNeume : public EditorToolkit {
public:
    EditorToolkitNeume(Doc * doc, View *view) : EditorToolkit(doc, view) {}
    bool ParseEditorAction(const std::string &json_editorAction);
    virtual std::string EditInfo() { return m_infoObject.json(); };

    /**
     * Experimental editor functions.
     */
    ///@{
    bool Chain(jsonxx::Array actions);
    bool Drag(std::string elementId, int x, int y);
    bool Insert(std::string elementType, std::string staffId, int ulx, int uly,
        int lrx, int lry, std::vector<std::pair<std::string, std::string>> attributes);
    bool Merge(std::vector<std::string> elementIds);
    bool Set(std::string elementId, std::string attrType, std::string attrValue);
    bool SetText(std::string elementId, std::string text);
    bool SetClef(std::string elementId, std::string shape);
    bool Split(std::string elementId, int x);
    bool Remove(std::string elementId);
    bool Resize(std::string elementId, int ulx, int uly, int lrx, int lry);
    bool Group(std::string groupType, std::vector<std::string> elementIds);
    bool Ungroup(std::string groupType, std::vector<std::string> elementIds);
    bool ChangeGroup(std::string elementId, std::string contour);
    bool ToggleLigature(std::vector<std::string> elementIds, std::string isLigature);
    bool ChangeSkew(std::string elementId, int dy, bool rightSide);
    bool ChangeStaff(std::string elementId);
    ///@}
protected:

    /**
     * Parse JSON instructions for experimental editor functions.
     */
    ///@{
    bool ParseDragAction(jsonxx::Object param, std::string *elementId, int *x, int *y);
    bool ParseInsertAction(jsonxx::Object param, std::string *elementType, std::string *startId, std::string *endId);
    bool ParseInsertAction(jsonxx::Object param, std::string *elementType, std::string *staffId, int *ulx, int *uly,
        int *lrx, int *lry, std::vector<std::pair<std::string, std::string>> *attributes);
    bool ParseMergeAction(jsonxx::Object param, std::vector<std::string> *elementIds);
    bool ParseSetAction(jsonxx::Object param, std::string *elementId, std::string *attrType, std::string *attrValue);
    bool ParseSetTextAction(jsonxx::Object param, std::string *elementId, std::string *text);
    bool ParseSetClefAction(jsonxx::Object param, std::string *elementId, std::string *shape);
    bool ParseSplitAction(jsonxx::Object param, std::string *elementId, int *x);
    bool ParseRemoveAction(jsonxx::Object param, std::string *elementId);
    bool ParseResizeAction(jsonxx::Object param, std::string *elementId, int *ulx, int *uly, int *lrx, int *lry);
    bool ParseGroupAction(jsonxx::Object param, std::string *groupType, std::vector<std::string> *elementIds);
    bool ParseUngroupAction(jsonxx::Object param, std::string *groupType, std::vector<std::string> *elementIds);
    bool ParseChangeGroupAction(jsonxx::Object param, std::string *elementId, std::string *contour);
    bool ParseToggleLigatureAction(jsonxx::Object param, std::vector<std::string> *elementIds, std::string *isLigature);
    bool ParseChangeSkewAction(jsonxx::Object param, std::string *elementId, int *dy, bool *rightSide);
    bool ParseChangeStaffAction(jsonxx::Object param, std::string *elementId);
    ///@}

private:
    jsonxx::Object m_infoObject;
};

//--------------------------------------------------------------------------------
// Comparator structs
//--------------------------------------------------------------------------------
// To be used with std::sort to find the object with a closest bounding
// box to a point defined by the x and y parameters of ClosestBB

struct ClosestBB {
    int x;
    int y;

    int distanceToBB(int ulx, int uly, int lrx, int lry, double skew = 0)
    {
        int offset = (x - ulx) * tan(skew * M_PI / 180.0);
        uly = uly - offset;
        lry = lry - offset;
        int xDiff = std::max(
                (ulx > x ? ulx - x : 0),
                (x > lrx ? x - lrx : 0)
        );
        int yDiff = std::max(
                (uly > y ? uly - y : 0),
                (y > lry ? y - lry : 0)
        );

        return sqrt(xDiff * xDiff + yDiff * yDiff);
    }

    bool operator() (Object *a, Object *b) {
        if (!a->GetFacsimileInterface() || !b->GetFacsimileInterface()) return true;
        Zone *zoneA = a->GetFacsimileInterface()->GetZone();
        Zone *zoneB = b->GetFacsimileInterface()->GetZone();

        int distA = distanceToBB(zoneA->GetUlx(), zoneA->GetUly(), zoneA->GetLrx(), zoneA->GetLry(), zoneA->GetSkew());
        int distB = distanceToBB(zoneB->GetUlx(), zoneB->GetUly(), zoneB->GetLrx(), zoneB->GetLry(), zoneB->GetSkew());
        return (distA < distB);
    }
};

// To be used with std::stable_sort to find the position to insert a new staff

struct StaffSort {
    // Sort staves left-to-right and top-to-bottom
    // Sort by y if there is no intersection, by x if there is
    bool operator() (Object *a, Object *b) {
        if (!a->GetFacsimileInterface() || !b->GetFacsimileInterface()) return true;
        Zone *zoneA = a->GetFacsimileInterface()->GetZone();
        Zone *zoneB = b->GetFacsimileInterface()->GetZone();

        // Check for y intersection
        if ((zoneA->GetUly() < zoneB->GetLry() && zoneA->GetLry() > zoneB->GetLry()) ||
            (zoneA->GetUly() < zoneB->GetUly() && zoneA->GetLry() > zoneB->GetUly())) {
            // y intersection, so sort by ulx
            return (zoneA->GetUlx() < zoneB->GetUlx());
        }
        else { // no intersection
            return (zoneA->GetUly() < zoneB->GetUly());
        }
    }
};

} // namespace vrv

#endif
