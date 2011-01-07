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

////////////////////////////////////////////////////////////////////////////////
// Zenith Engine
// File Name : IGraph.h
// Creation : 22/06/2007
// Author : Cedric Guillemet
// Description : Graph Node Interface
////////////////////////////////////////////////////////////////////////////////

#ifndef GRAPH_H__
#define GRAPH_H__

///////////////////////////////////////////////////////////////////////////////////////////////////

class wxGraphContainer;

///////////////////////////////////////////////////////////////////////////////////////////////////
/*
Trucs a faire:
- definir des props en lua qui seront serilisées en xml/binary (classe scripttable?)
-> extension d1 classe c++ par lua????

- on ne pas pas rajouter, 1 final node quand yen a deja 1
- quand on supprime 1 final node, le container n'a plus de ref dessus
- il faut 1 final node pour generer le truc

- coder les differents nodes (au moins 1 ptit paquet)
- serialisation (xml/binary)
- generation cgfx + compilation (possible avant integration avec le moteur)
- affichage du resultat (apres le render target/swpachain du moteur)
- edition des proprietes de nodes (declarées en lua) + serialisation (avec le node inspector)
ex:
nNode:persistent = {blablabla}
print("props Z = "..nNode:persistent.z)
persistent: objet c++avec serialisable?
- verifier le refcount de swig
*/
///////////////////////////////////////////////////////////////////////////////////////////////////

class wxGraphNode
{
public:
    virtual void SetFunctionName(const char * sFunction) = 0;
    virtual const char *GetFunctionName() = 0;

    virtual void AddRightPlug(const char * sPlugName, int aCon = 0) = 0;
    virtual void AddLeftPlug(const char * sPlugName, int aCon = 0) = 0;

    virtual unsigned int GetNbRightPlugs() = 0;
    virtual unsigned int GetNbLeftPlugs() = 0;
    virtual const char * GetRightPlugName(unsigned int aPlugIndex) = 0;
    virtual const char * GetLeftPlugName(unsigned int aPlugIndex) = 0;

    virtual void SetScript(const char *script) = 0;
    virtual void SetFinalNode() = 0;

    // /!\ UNEXPORTED FUNCTIONS MUST BE @ THE END OF THE CLASS AND NO INHERITS ALLOWED AFTERWARDS
    virtual void AddThumbnail(unsigned int sizeX, unsigned int sizeY) = 0;
};

class wxGraphContainer
{
public:
    virtual wxGraphNode *AddNode() = 0;
    virtual void AddNodePattern(const char *szPatternName, const char *szCreationScriptFunction) = 0;

    virtual unsigned int GetPatternCount() = 0;
    virtual const char * GetPatternName(unsigned int idx) = 0;
    virtual const char * GetPatternScriptFunction(unsigned int idx) = 0;
#ifdef _DEBUG
    virtual void DumpConnections() = 0;
#endif
};

//wxGraphContainer * GetContainer();

///////////////////////////////////////////////////////////////////////////////////////////////////


#endif
