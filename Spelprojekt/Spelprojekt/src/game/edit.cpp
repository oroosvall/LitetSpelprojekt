#include "edit.h"

Edit::~Edit()
{
	delete current;
	delete currentLight;
}

void Edit::init(Map* map, UserInput* in)
{
	editContentID = -1;
	discard();

	this->map = map;
	chunks = map->getChunks();
	this->in = in;
	this->width = map->readSizeX();
	this->height = map->readSizeY();
	editMode = EditMode::NONEM;
	editModeLast = EditMode::NONEM;
	editState = EditState::NONES;
	editStateLast = EditState::NONES;

	lastMousePosX = 0;
	lastMousePosY = 0;
	current = 0;
}

void Edit::refreshOnEnter()
{
	editMode = NONEM;
	editState = NONES;
	editStateLast = NONES;
}

void Edit::update(float x, float y, GUI* gui)
{
	if (editMode != NONEM)
	{
		map->getChunkIndex(*in->GetPos(), &chunkXCam, &chunkYCam);
		EditorMode();
	}

	if (in->getKeyNumberState(7))
		forceRekts = true;
	else if (in->getKeyNumberState(8))
		forceRekts = false;

	if (editState != editStateLast)
	{
		if (editState == EditState::CHANGE)
			gui->fixEditorSwitches(false, true);
		else if (editState == EditState::PLACE)
			gui->fixEditorSwitches(true, false);
	}

	editModeLast = editMode;
	editStateLast = editState;
	placeStateLast = placeState;
}

void Edit::EditorMode()
{
	//If edit mode was changed
	if (editMode != editModeLast)
	{
		placeState = PlaceState::NONEP;
		internalPlaceState = 0;
		editContentID = -1;
		discard();
		itemtaken = false;
		itemPlaced = false;
		forceRekts = false;
	}
	
	switch (editMode)
	{
	case BACK:
	case WORLD:
	case MONSTER:
	{
		//hold item code
		itemTableClick();

		if (editState != NONES)
			EditorState();
		break;
	}		
	case REKT:
		RektEdit();
		break;
	case LIGHT:
		if (editState != NONES)
			EditorState();
		break;
	}
}

void Edit::itemTableClick()
{
	if (in->getLMBrelease())
	{
		float x, y;
		bool l, r;
		in->getMouseState(&x, &y, &r, &l);

		if (x > 247 && x < 535 && y > 596 && y < 717)
		{
			itemtaken = false;

			int indexX = ((int)x - 247) / 24;
			int indexY = ((int)y - 596) / 24;

			editContentID = 12 * indexY + indexX;

			discard();
			HoldNewItem();
		}
	}
}

void Edit::RektEdit()
{
	forceRekts = true;
	//only if we are holding down a key
	int action = -1;
	if (in->getLMBdown())
		action = 1; //add
	if (in->getRMBdown())
		action = 0; // remove
	if (action == -1)
		return;

	float x, y;
	bool l, r;
	in->getMouseState(&x, &y, &r, &l);

	if (y > 560) //in hud
		return; 

	mouseToWorld(&x, &y);

	map->getChunkIndex(*in->GetPos(), &chunkXCam, &chunkYCam);
	map->getChunkIndex(*in->GetPos(), &chunkXMouse, &chunkYMouse);

	if (chunkXCam != chunkXMouse || chunkYCam != chunkYMouse)
		return;

	if (chunkXCam > -1 && chunkXCam < map->readSizeX() &&
		chunkYCam > -1 && chunkYCam < map->readSizeY())
	{
		int xindex = int(x + 17.5f) % 35;
		int yindex = -(int(y - 17.5f) % 35);

		if (xindex > -1 && xindex < 35 && yindex > -1 && yindex < 35)
		{
			//remove
			if (action == 0)
				chunks[chunkXCam][chunkYCam].removeRekt(xindex, yindex);
			//add
			else if (action == 1)
				chunks[chunkXCam][chunkYCam].addRekt(xindex, yindex);
		}
	}
}

void Edit::EditorState()
{
	//if edit state was changed
	if (editState == EditState::CHANGE && editStateLast != EditState::CHANGE)
	{
		discard();
		editContentID = -1;
		placeState = PlaceState::NONEP;
		internalPlaceState = 0;
	}
	
	float x, y;
	bool lmb, rmb;
	in->getMouseState(&x, &y, &rmb, &lmb);

	if (editState == EditState::PLACE)
	{
		if(editContentID != -1 || editMode == EditMode::LIGHT)
		{
			if (in->getKeyNumberState(5))
				newItem = true;
			if (in->getKeyNumberState(6))
			{
				newItem = true;
				coppyLast = true;
			}
			if (newItem)
				HoldNewItem();

			if (current || currentLight)
			{
				PlaceEditorState(x, y);
				if (in->getEreleased())
					giveObjectToChunk();
			}
		}
	}
	else if (editState == EditState::CHANGE)
	{
		//grab item
		if (in->getLMBrelease())
		{
			if (y < 560)
			{
				discard();
				editContentID = -1;
				itemtaken = true;
				itemPlaced = false;
				if (editMode == EditMode::LIGHT)
				{
					currentLight = chunks[chunkXCam][chunkYCam].takeClosestLight(*in->GetPos());
					if(currentLight)
						takenCopyLight = *currentLight;
					else
						itemtaken = false;
				}
				else
				{
					current = chunks[chunkXCam][chunkYCam].takeClosestWorldItem(*in->GetPos());
					if (current) // if null wasnt returend
					{
						editContentID = current->returnID();
						takenCopy.init(editContentID);
						takenCopy.coppyMat(current);
					}
					else
						itemtaken = false;
				}
				if (!in->getKeyState('Q')) //change into place mode
				{
					editState = EditState::PLACE;
				}
			}
		}
	}
}

void Edit::HoldNewItem()
{
	discard();

	internalPlaceState = 0;
	placeState = NONEP;
	editState = PLACE;
	newItem = false;
	if (coppyLast)
	{
		coppyLast = false;
		if (itemtaken)
		{
			if (editMode == EditMode::LIGHT)
			{
				currentLight = new Light();
				*currentLight = takenCopyLight;
			}
			else
			{
				current = new GameObject();
				current->init(takenCopy.returnID());
				current->coppyMat(&takenCopy);
			}
		}
		else if (itemPlaced && editMode != EditMode::LIGHT)
		{
			current = new GameObject();
			current->init(lastPlaced.returnID());
			current->coppyMat(&lastPlaced);
		}
		else if (itemPlaced && editMode == EditMode::LIGHT)
		{
			currentLight = new Light();
			*currentLight = lastPlacedLight;
		}
	}
	else
	{	
		if (editMode == EditMode::LIGHT)
		{
			currentLight = new Light();
			currentLight->init(in->GetPos()->x, in->GetPos()->y);
		}
		else
		{
			current = new GameObject();
			current->init(editContentID);
			current->moveTo(in->GetPos()->x, in->GetPos()->y, 0);
		}
	}
}

void Edit::PlaceEditorState(float x, float y)
{
	if (y < 560)
	{
		//convert mouse x, y to world
		mouseToWorld(&x, &y);

		//get current chunk index edit
		map->getChunkIndex(*in->GetPos(), &chunkXCam, &chunkYCam);

		if (chunkXCam == -1 || chunkXMouse == -1)
			return;
		
		placeObject(x, y);
	}
}

void Edit::placeObject(float x, float y)
{
	//Set Place State
	if (in->getKeyNumberState(1))
		placeState = NONEP;
	if (in->getKeyNumberState(2))
	{
		placeState = MOVE;
		internalPlaceState = 0;
	}
	else if (in->getKeyNumberState(3))
	{
		placeState = SCALE;
		internalPlaceState = 0;
	}
	else if (in->getKeyNumberState(4))
	{
		placeState = ROT;
		internalPlaceState = 0;
	}
	if (placeState != placeStateLast)
		internalPlaceState = 0;

	if (in->getLMBrelease())
		internalPlaceState++;

	if (in->getRMBdown() || in->updateMouse())
	{

	}
	else //update if lmb or shift isnt down
	{
		switch (placeState)
		{
		case MOVE:
			if (internalPlaceState == 0)
			{
				if (editMode == EditMode::LIGHT)
				{
					if (in->getKeyState('Q'))
						currentLight->translate( 0 , 0 , y - lastMousePosY);
					else
						currentLight->translate(x - lastMousePosX, y - lastMousePosY, 0);
				}
				else
				{
					if (in->getKeyState('Q'))
						current->translateSNAPXY(x - lastMousePosX, y - lastMousePosY);
					else
						current->translateEDITOR(x - lastMousePosX, y - lastMousePosY, 0);
				}
			}
			else if (internalPlaceState == 1)
			{
				if (editMode == EditMode::LIGHT)
				{
					internalPlaceState = 0;
					placeState = NONEP;
				}
				else if (in->getKeyState('Q'))
					current->translateSNAPZ(y - lastMousePosY);
				else
					current->translateEDITOR(0, 0, y - lastMousePosY);

			}
			else if (internalPlaceState == 2)
			{
				placeState = NONEP;
				internalPlaceState = 0;
			}
			break;
		case SCALE:
			if (internalPlaceState == 0)
			{
				if (editMode == EditMode::LIGHT)
				{
					if (in->getKeyState('Q'))
					{
						if (in->getKeyState('A'))
							currentLight->scale(0, 0, 1);
						else if (in->getKeyState('S'))
							currentLight->scale(0, 0, 2);
						else if (in->getKeyState('D'))
							currentLight->scale(0, 0, 3);
					}		
					else
						currentLight->scale(x - lastMousePosX, y - lastMousePosY, 0);
				}
				else
				{
					if (in->getKeyState('Q'))
						current->scaleSNAP(x - lastMousePosX, y - lastMousePosY, 0);
					else
						current->scaleAD(x - lastMousePosX, y - lastMousePosY, 0);
				}
			}

			else if (internalPlaceState == 1)
			{
				if (editMode == EditMode::LIGHT)
				{
					internalPlaceState = 0;
					placeState = NONEP;
				}
				else if (in->getKeyState('Q'))
					current->scaleSNAP(0, 0, y - lastMousePosY);
				else
					current->scaleAD(0, 0, y - lastMousePosY);
			}

			else if (internalPlaceState == 2)
			{
				placeState = NONEP;
				internalPlaceState = 0;
			}
			break;
		case ROT:
			if (internalPlaceState == 0)
			{
				if (editMode == EditMode::LIGHT)
				{
					if (in->getKeyState('Q'))
						currentLight->color(0, 0, y - lastMousePosY);
					else
						currentLight->color(x - lastMousePosX, y - lastMousePosY, 0);
				}
				else
					current->rotateToX((y - lastMousePosY) * 0.8f);

			}
			else if (internalPlaceState == 1)
			{
				if (editMode == EditMode::LIGHT)
				{
					internalPlaceState = 0;
					placeState = NONEP;
				}
				else
					current->rotateToY((x - lastMousePosX) * 0.8f);
			}

			else if (internalPlaceState == 2)
				if (editMode != EditMode::LIGHT)
					current->rotateToZ((y - lastMousePosY) * 0.8f);

			else if (internalPlaceState == 3)
			{
				placeState = NONEP;
				internalPlaceState = 0;
			}
			break;
		}
	}
	lastMousePosX = x;
	lastMousePosY = y;
}

void Edit::giveObjectToChunk()
{

	switch (editMode)
	{
	case BACK:
		break;
	case WORLD:
	{
		int idx, idy;
		map->getChunkIndex(current->readPos(), &idx, &idy);
		if (idx == -1 || idy == -1)
			chunks[chunkXCam][chunkYCam].recieveWorld(current);
		else
			chunks[idx][idy].recieveWorld(current);
		lastPlaced = *current;
		current = 0;
		newItem = true;
		itemPlaced = true;
		itemtaken = false;
	}
		break;
	case MONSTER:
		break;
	case REKT:
		break;
	case LIGHT:
	{
		int idx, idy;
		map->getChunkIndex(vec3(currentLight->posX, currentLight->posY, currentLight->posZ), &idx, &idy);
		if (idx == -1 || idy == -1)
			chunks[chunkXCam][chunkYCam].recieveLight(currentLight);
		else
			chunks[idx][idy].recieveLight(currentLight);

		lastPlacedLight = *currentLight;
		discard();
		currentLight = 0;
		newItem = true;
		itemtaken = false;
		itemPlaced = true;
	}
		break;
	}
}

void Edit::discard()
{
	delete current;
	current = 0;

	delete currentLight;
	currentLight = 0;
}

Edit::EditMode Edit::getEditMode()
{
	return editMode;
}

void Edit::mouseToWorld(float* x, float* y)
{
	mouseToScreenSpace(x, y);
	glm::vec3* temp = in->GetPos();
	*x = temp->x + (*x * 14.0f);
	*y = temp->y + (*y * 9.5f);
}

void Edit::mouseToScreenSpace(float* x, float* y)
{
	*x = (*x / (SCREENWIDTH * 0.5f)) - 1.0f;
	*y = ((*y / (SCREENHEIGHT * 0.5f)) - 1.0f) * -1.0f;
}

void Edit::guiHandle(int bEvent)
{
	switch (bEvent)
	{
		//EDIT MODE BUTTON SET

	case(100) :
		editMode = EditMode::WORLD;
		editContentID = -1;
		break;
	case(101) : 
		editMode = EditMode::REKT;
		editContentID = -1;
		break;
	case(102) : 
		editMode = EditMode::LIGHT;
		editContentID = -1;
		break;
	case(103) :
		editMode = EditMode::MONSTER;
		editContentID = -1;
		break;
	case(104) :
		editMode = EditMode::BACK;
		editContentID = -1;
		break;
	case(105) : 
		editMode = EditMode::SHRINE;
		editContentID = -1;
		break;
	case(106) : 
		editMode = EditMode::BOSS;
		editContentID = -1;
		break;
	case(107) :
		editMode = EditMode::AUDIO;
		editContentID = -1;
		break;
	case(108) :
		editMode = EditMode::NONEM;
		editContentID = -1;
		break;
		//EDIT STATE SET

	case(110) :
		editState = EditState::PLACE;
		break;
	case(111) :
		editState = EditState::CHANGE;
		break;
	case(112) :
		editState = EditState::NONES;
		break;
	}
}

GameObject* Edit::getObject()
{
	return current;
}

void Edit::invalidID()
{
	discard();
	editContentID = -1;
}

Light* Edit::getLight()
{
	return currentLight;
}

bool Edit::isMovingLights()
{
	if (editMode == EditMode::LIGHT && editState == EditState::CHANGE)
		return true;
	return false;
}

void Edit::saveloadCheck(bool* save, bool* load, int* nr)
{
	float x, y;
	bool r, l;
	in->getMouseState(&x, &y, &r, &l);
	if (in->getLMBrelease())
	{
		if (x > 887 && x < 996 && y > 584 && y < 697)
		{
			int idx = (int)(x - 887);
			idx = idx / 58;

			int idy = (int)(y - 584);
			idy = idy / 24;

			*nr = idy;

			if (idx == 0) //load
			{
				*save = false;
				*load = true;
			}
			else if (idx == 1) // save
			{
				*save = true;
				*load = false;
			}
			return;
		}
	}
	*save = false;
	*load = false;
}