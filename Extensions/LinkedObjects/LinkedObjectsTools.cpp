/**

Game Develop - LinkedObjects Extension
Copyright (c) 2011 Florian Rival (Florian.Rival@gmail.com)

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
    claim that you wrote the original software. If you use this software
    in a product, an acknowledgment in the product documentation would be
    appreciated but is not required.

    2. Altered source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.

    3. This notice may not be removed or altered from any source
    distribution.

*/

#include "LinkedObjectsTools.h"
#include <iostream>
#include <string>
#include <boost/weak_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include "GDL/Object.h"
#include "GDL/RuntimeScene.h"
#include "ObjectsLinksManager.h"

using namespace std;

namespace GDpriv
{
namespace LinkedObjects
{

std::map < RuntimeScene* , ObjectsLinksManager > ObjectsLinksManager::managers;

bool GD_EXTENSION_API PickAllObjectsLinkedTo(RuntimeScene & scene, std::map <std::string, std::vector<Object*> *> mapOfAllObjectLists, std::vector<std::string> & alreadyDeclaredObjects, const std::string &, Object * object)
{
    //Get a list of all objects linked
    std::vector<Object*> linkedObjects = ObjectsLinksManager::managers[&scene].GetAllRawPointersToObjectsLinkedWith(object->Shared_ptrFromObject());

    //Then pick all of these objects
    for (unsigned int j = 0;j<linkedObjects.size();++j)
    {
        const std::string & linkedObjectName = linkedObjects[j]->GetName();

        //Add linked object to appropriate picked object list
        if ( mapOfAllObjectLists.find(linkedObjectName) != mapOfAllObjectLists.end() )
        {
            if ( find(mapOfAllObjectLists[linkedObjectName]->begin(), mapOfAllObjectLists[linkedObjectName]->end(), linkedObjects[j]) == mapOfAllObjectLists[linkedObjectName]->end() )
                mapOfAllObjectLists[linkedObjectName]->push_back(linkedObjects[j]);
        }

        //Add linked object to already concerned objects
        if ( find(alreadyDeclaredObjects.begin(), alreadyDeclaredObjects.end(), linkedObjectName) == alreadyDeclaredObjects.end() )
            alreadyDeclaredObjects.push_back(linkedObjectName);
    }

    return !linkedObjects.empty();
}

bool GD_EXTENSION_API PickObjectsLinkedTo(RuntimeScene & scene, std::map <std::string, std::vector<Object*> *> pickedObjectsLists, std::vector<std::string> & alreadyDeclaredObjects, const std::string &, Object * object, std::string linkedName)
{
    //Get a list of all objects with the desired name linked to our object
    std::vector<Object*> linkedObjects = ObjectsLinksManager::managers[&scene].GetRawPointersToObjectsLinkedWith(object->Shared_ptrFromObject(), linkedName);

    //Then pick all of these objects
    for (unsigned int j = 0;j<linkedObjects.size();++j)
    {
        const std::string & linkedObjectName = linkedObjects[j]->GetName();

        //Add linked object to appropriate picked object list
        if ( pickedObjectsLists.find(linkedObjectName) != pickedObjectsLists.end() )
        {
            if ( find(pickedObjectsLists[linkedObjectName]->begin(), pickedObjectsLists[linkedObjectName]->end(), linkedObjects[j]) == pickedObjectsLists[linkedObjectName]->end() )
                pickedObjectsLists[linkedObjectName]->push_back(linkedObjects[j]);
        }

    }
    //Add linked object to already concerned objects
    if ( find(alreadyDeclaredObjects.begin(), alreadyDeclaredObjects.end(), linkedName) == alreadyDeclaredObjects.end() )
        alreadyDeclaredObjects.push_back(linkedName);

    return !linkedObjects.empty();
}

void GD_EXTENSION_API LinkObjects(RuntimeScene & scene, Object * a, Object * b, const std::string & ,const std::string & )
{
    ObjectsLinksManager::managers[&scene].LinkObjects(a->Shared_ptrFromObject(),b->Shared_ptrFromObject());
}

void GD_EXTENSION_API RemoveLinkBetween(RuntimeScene & scene, Object * a, Object * b, const std::string & ,const std::string & )
{
    ObjectsLinksManager::managers[&scene].RemoveLinkBetween(a->Shared_ptrFromObject(),b->Shared_ptrFromObject());
}

void GD_EXTENSION_API RemoveAllLinksOf(RuntimeScene & scene, Object * object)
{
    ObjectsLinksManager::managers[&scene].RemoveAllLinksOf(object->Shared_ptrFromObject());
}

}
}