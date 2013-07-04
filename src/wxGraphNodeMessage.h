/*
 *                            COPYRIGHT
 *
 * This is a part of Rapido!
 *
 *  Copyright (C) 2006 Cedric Guillemet
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef WXGRAPHNODEMESSAGE_H__
#define WXGRAPHNODEMESSAGE_H__

#include "wxGraphNode.h"

class wxGraphNodeMessage : public wxGraphNode
{
private:
	typedef wxGraphNode inherited;

public:
	wxGraphNodeMessage(wxGraphContainer* parent);
	virtual ~wxGraphNodeMessage();

	virtual bool OnAddNewConnection(wxGraphNode *pOther);

	virtual GraphNodeType GetType() { return GNT_MESSAGE; }

	virtual TiXmlNode* CreateLegacyXmlNodeWithChildren();
	virtual TiXmlNode* CreateXmlNodeWithChildren();
	virtual void ParseXmlElement(TiXmlElement* aXmlElement);

protected:
	virtual void OnPaint(wxPaintEvent& event);

};

#endif
