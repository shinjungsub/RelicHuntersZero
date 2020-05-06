#include "stdafx.h"
#include "LoadingScene.h"


void LoadingScene::Init(void)
{
	//로딩클래스 초기화
	_loading = new Loading;
	_loading->Init();

	//이미지 및 사운드 로딩
	this->LoadingImage();
	this->LoadingSound();
}

void LoadingScene::Release(void)
{
	//로딩클래스 해제
	_loading->Release();
	SAFE_DELETE(_loading);
}

void LoadingScene::Update(void)
{
	//로딩클래스 업데이트
	_loading->Update();

	//로딩완료후 화면(씬)변경
	if (_loading->loadingDone())
	{
		SCENEMANAGER->LoadScene("Stage");
	}
}

void LoadingScene::Render(void)
{
	//로딩클래스 렌더
	_loading->Render();

	float num = _loading->getLoadItem().size();
	float cur = _loading->getCurrentGauge();
	float per = cur / num * 100;
	char str[128];
	sprintf(str, "%.f %%", per);
	wstring w_str;
	for (int i = 0; i < 128; i++)
	{
		if (str[i] == '\0')
		{
			break;
		}
		w_str.push_back(str[i]);
	}
	D2DRENDERER->RenderText(600, 370, w_str, 20, D2DRenderer::DefaultBrush::Red);

	if (_loading->getCurrentGauge() < _loading->getLoadItem().size())
	{
		if (_loading->getLoadItem()[_loading->getCurrentGauge()]->getLoadingKind() == LOAD_KIND::LOAD_KIND_IMAGE ||
			_loading->getLoadItem()[_loading->getCurrentGauge()]->getLoadingKind() == LOAD_KIND::LOAD_KIND_FRAMEIMAGE)
		{
			string str1 = ".png";
			string total;
			total = _loading->getLoadItem()[_loading->getCurrentGauge()]->getImageResource().keyName + str1;
			wstring w_str1;
			w_str1.assign(total.begin(), total.end());
			D2DRENDERER->RenderText(600, 330, w_str1, 20, D2DRenderer::DefaultBrush::Red);
		}
		/*else if (_loading->getLoadItem()[_loading->getCurrentGauge()]->getLoadingKind() == LOAD_KIND::LOAD_KIND_SOUND)
		{
			string str2 = ".ogg";
			string total;
			total = _loading->getLoadItem()[_loading->getCurrentGauge()]->getSoundResource().keyName + str2;
			wstring w_str1;
			w_str1.assign(total.begin(), total.end());
			D2DRENDERER->RenderText(600, 330, w_str1, 20, D2DRenderer::DefaultBrush::Red);
		}*/
	}
}

void LoadingScene::LoadingImage()
{
	/////////////
	/* Bullet */
	/////////////
	_loading->loadImage("spr_bullet_0", L"./Graphics/Bullets/spr_bullet_0.png");
	_loading->loadImage("spr_bullet_1", L"./Graphics/Bullets/spr_bullet_1.png");
	_loading->loadImage("spr_bullet_blue_1", L"./Graphics/Bullets/spr_bullet_blue_1.png");
	_loading->loadImage("spr_bullet_critical_1", L"./Graphics/Bullets/spr_bullet_critical_1.png");
	_loading->loadImage("spr_bullet_small_1", L"./Graphics/Bullets/spr_bullet_small_1.png");
	_loading->loadImage("spr_bullet_stronger_0", L"./Graphics/Bullets/spr_bullet_stronger_0.png");
	_loading->loadImage("spr_casing_0", L"./Graphics/Bullets/spr_casing_0.png");
	_loading->loadImage("spr_casing_1", L"./Graphics/Bullets/spr_casing_1.png");
	_loading->loadImage("spr_casing_2", L"./Graphics/Bullets/spr_casing_2.png");
	_loading->loadImage("spr_rocket_0", L"./Graphics/Bullets/spr_rocket_0.png");
	_loading->loadFrameImage("spr_bullet_plasma", L"./Graphics/Bullets/spr_bullet_plasma.png", 12, 1);
	_loading->loadFrameImage("spr_bullet_temp", L"./Graphics/Bullets/spr_bullet_temp.png", 12, 1);
	_loading->loadFrameImage("spr_bullet_yoodo", L"./Graphics/Bullets/spr_bullet_yoodo.png", 12, 1);

	/////////////
	/* Effect */
	/////////////
	_loading->loadImage("ExclamationMark", L"./Graphics/Effects/ExclamationMark.png");
	_loading->loadFrameImage("heavy_hit_effect", L"./Graphics/Effects/spr_hit_heavy_0-sheet.png", 3, 1);
	_loading->loadFrameImage("light_hit_effect", L"./Graphics/Effects/spr_hit_light_0-sheet.png", 3, 1);
	_loading->loadFrameImage("medium_hit_effect", L"./Graphics/Effects/spr_hit_medium_0-sheet.png", 3, 1);

	///////////////
	///* Enemy */
	///////////////

	//duck
	_loading->loadFrameImage("duck_idle", L"./Graphics/Enemies/duck2/spr_duck2_idle_0-sheet.png", 7, 1);
	_loading->loadFrameImage("duck_idle_left", L"./Graphics/Enemies/duck2/spr_duck2_idle_0-sheet_left.png", 7, 1);
	_loading->loadFrameImage("duck_walk", L"./Graphics/Enemies/duck2/spr_duck2_walk_0-sheet.png", 6, 1);
	_loading->loadFrameImage("duck_walk_left", L"./Graphics/Enemies/duck2/spr_duck2_walk_0-sheet_left.png", 6, 1);
	_loading->loadFrameImage("duck_hit", L"./Graphics/Enemies/duck2/spr_duck2_hit_0-sheet.png", 2, 1);
	_loading->loadFrameImage("duck_hit_left", L"./Graphics/Enemies/duck2/spr_duck2_hit_0-sheet_left.png", 2, 1);
	_loading->loadFrameImage("duck_death", L"./Graphics/Enemies/duck2/spr_duck2_death2_0-sheet.png", 11, 1);
	_loading->loadFrameImage("duck_death_left", L"./Graphics/Enemies/duck2/spr_duck2_death2_0-sheet_left.png", 11, 1);
	_loading->loadFrameImage("duck_dash", L"./Graphics/Enemies/duck2/spr_duck2_dash.png", 2, 1);
	_loading->loadFrameImage("duck_dash_left", L"./Graphics/Enemies/duck2/spr_duck2_dash_left.png", 2, 1);

	///////////////
	///* Hud */
	///////////////
	_loading->loadImage("HpBarBackground", L"./Graphics/Huds/objecthpB.png");
	_loading->loadImage("HpBarForeground", L"./Graphics/Huds/objecthp.png");
	_loading->loadImage("crosshair_0", L"./Graphics/Huds/crosshair_0.png");
	_loading->loadImage("crosshair_2", L"./Graphics/Huds/crosshair_2.png");

	///////////////
	///* Item */
	///////////////
	_loading->loadImage("pistol_0", L"./Graphics/Items/Weapons/pistol/pistol_0.png");	//62, 26
	_loading->loadImage("pistol_0_left", L"./Graphics/Items/Weapons/pistol/pistol_0_left.png");
	_loading->loadImage("machinegun_0", L"./Graphics/Items/Weapons/machinegun/machinegun_0.png");	//72, 28
	_loading->loadImage("machinegun_0_left", L"./Graphics/Items/Weapons/machinegun/machinegun_0_left.png");
	_loading->loadImage("shotgun_0", L"./Graphics/Items/Weapons/shotgun1/shotgun_0.png");	//76, 20
	_loading->loadImage("shotgun_0_left", L"./Graphics/Items/Weapons/shotgun1/shotgun_0_left.png");

	/////////////
	/* Player */
	/////////////
	_loading->loadImage("pinky_dashsfx_left", L"./Graphics/Players/pinky/pinky_dashsfx_left.png");
	_loading->loadImage("pinky_dashsfx", L"./Graphics/Players/pinky/pinky_dashsfx_right.png");
	_loading->loadFrameImage("pinky_dash_left", L"./Graphics/Players/pinky/pinky_dash_left.png", 2, 1);
	_loading->loadFrameImage("pinky_dash", L"./Graphics/Players/pinky/pinky_dash_right.png", 2, 1);
	_loading->loadFrameImage("pinky_dashsfx_left", L"./Graphics/Players/pinky/pinky_dashsfx_left_!.png", 2, 1);
	_loading->loadFrameImage("pinky_dashsfx", L"./Graphics/Players/pinky/pinky_dashsfx_right_!.png", 2, 1);
	_loading->loadFrameImage("pinky_death_left", L"./Graphics/Players/pinky/pinky_death_left.png", 11, 1);
	_loading->loadFrameImage("pinky_death", L"./Graphics/Players/pinky/pinky_death_right.png", 11, 1);
	_loading->loadFrameImage("pinky_dig_left", L"./Graphics/Players/pinky/pinky_dig_left.png", 3, 1);
	_loading->loadFrameImage("pinky_dig", L"./Graphics/Players/pinky/pinky_dig_right.png", 3, 1);
	_loading->loadFrameImage("pinky_hit_left", L"./Graphics/Players/pinky/pinky_hit_left.png", 2, 1);
	_loading->loadFrameImage("pinky_hit", L"./Graphics/Players/pinky/pinky_hit_right.png", 2, 1);
	_loading->loadFrameImage("pinky_holo_left", L"./Graphics/Players/pinky/pinky_holo_left.png", 27, 1);
	_loading->loadFrameImage("pinky_holo", L"./Graphics/Players/pinky/pinky_holo_right.png", 27, 1);
	_loading->loadFrameImage("pinky_idle_left", L"./Graphics/Players/pinky/pinky_idle_left.png", 9, 1);
	_loading->loadFrameImage("pinky_idle", L"./Graphics/Players/pinky/pinky_idle_right.png", 9, 1);
	_loading->loadFrameImage("pinky_melee_left", L"./Graphics/Players/pinky/pinky_melee_left.png", 7, 1);
	_loading->loadFrameImage("pinky_melee", L"./Graphics/Players/pinky/pinky_melee_right.png", 7, 1);
	_loading->loadFrameImage("pinky_sprint_left", L"./Graphics/Players/pinky/pinky_sprint_left.png", 6, 1);
	_loading->loadFrameImage("pinky_sprint", L"./Graphics/Players/pinky/pinky_sprint_right.png", 6, 1);
	_loading->loadFrameImage("pinky_teleport_in_left", L"./Graphics/Players/pinky/pinky_teleport_in_left.png", 8, 1);
	_loading->loadFrameImage("pinky_teleport_in", L"./Graphics/Players/pinky/pinky_teleport_in_right.png", 8, 1);
	_loading->loadFrameImage("pinky_teleport_out_left", L"./Graphics/Players/pinky/pinky_teleport_out_left.png", 8, 1);
	_loading->loadFrameImage("pinky_teleport_out", L"./Graphics/Players/pinky/pinky_teleport_out_right.png", 8, 1);
	_loading->loadFrameImage("pinky_walk_left", L"./Graphics/Players/pinky/pinky_walk_left.png", 6, 1);
	_loading->loadFrameImage("pinky_walk", L"./Graphics/Players/pinky/pinky_walk_right.png", 6, 1);

	/////////////
	/* Stage */
	/////////////
	//_loading->loadImage("", L"./Graphics/Stages/.png");
	//_loading->loadFrameImage("", L"./Graphics/Stages/.png", 0, 0);

	///////////////
	///* Struct */
	///////////////
	_loading->loadImage("struct1", L"./Graphics/Structs/boxes/sprite327_0.png");
	_loading->loadImage("struct2", L"./Graphics/Structs/boxes/sprite327_1.png");
	_loading->loadImage("struct3", L"./Graphics/Structs/boxes/sprite327_2.png");
	//_loading->loadFrameImage("", L"./Graphics/Structs/.png", 0, 0);

	///////////////
	///* Tile */
	///////////////
	_loading->loadImage("tile_1", L"./Graphics/Tiles/Stage1/tile_1.png");
	_loading->loadImage("tile_2", L"./Graphics/Tiles/Stage1/tile_2.png");
	//_loading->loadFrameImage("", L"./Graphics/Tiles/.png", 0, 0);
}

void LoadingScene::LoadingSound()
{
	//string key;
	//string fileName;
	//string fileType = ".ogg";
	//string keyTotal;
	//string fileNameTotal;
	////스타트씬
	//_loading->loadSound("Opening", "./SOUND/UI/Opening.ogg", false, false);
	//_loading->loadSound("Forest", "./SOUND/SFX/AMBIENCE/Forest.ogg", true, true);
	//_loading->loadSound("Motif", "./SOUND/UI/Motif.ogg", false, false);
	//_loading->loadSound("StartGame", "./SOUND/UI/StartGame.ogg", false, false);
	//_loading->loadSound("Move", "./SOUND/UI/Move.ogg", false, false);
	//_loading->loadSound("Select", "./SOUND/UI/Select.ogg", false, false);
	//_loading->loadSound("No", "./SOUND/UI/No.ogg", false, false);

	//_loading->loadSound("Overworld1", "./SOUND/MUSIC/WORLD/Overworld1.ogg", true, true);
	//_loading->loadSound("Yeti", "./SOUND/MUSIC/BOSS/Yeti.ogg", true, true);

	//_loading->loadSound("Crush", "./SOUND/PLAYER/Death/Crush.ogg", false, false);
	//_loading->loadSound("InWaterLoop", "./SOUND/PLAYER/InWaterLoop.ogg", false, false);
	////플레이어 걷는 소리들
	//for (int i = 1; i < 8; i++)
	//{
	//	key = "Water";
	//	fileName = "./SOUND/PLAYER/Water/Step";
	//	keyTotal = key + to_string(i);
	//	fileNameTotal = fileName + to_string(i) + fileType;
	//	_loading->loadSound(keyTotal, fileNameTotal, false, false);

	//	key = "Grass";
	//	fileName = "./SOUND/PLAYER/Grass/Step";
	//	keyTotal = key + to_string(i);
	//	fileNameTotal = fileName + to_string(i) + fileType;
	//	_loading->loadSound(keyTotal, fileNameTotal, false, false);

	//	key = "Vine";
	//	fileName = "./SOUND/PLAYER/Vine/Step";
	//	keyTotal = key + to_string(i);
	//	fileNameTotal = fileName + to_string(i) + fileType;
	//	_loading->loadSound(keyTotal, fileNameTotal, false, false);

	//	key = "Stone";
	//	fileName = "./SOUND/PLAYER/Stone/Step";
	//	keyTotal = key + to_string(i);
	//	fileNameTotal = fileName + to_string(i) + fileType;
	//	_loading->loadSound(keyTotal, fileNameTotal, false, false);
	//}

	////플레이어
	//_loading->loadSound("Arrow_Charge", "./SOUND/PLAYER/arrow_charge.ogg", false, false);
	////화살 돌아오기
	//_loading->loadSound("Arrow_Retrieval", "./SOUND/PLAYER/arrow_retrieval.ogg", false, false);
	//_loading->loadSound("Arrow_Shoot", "./SOUND/PLAYER/arrow_shoot.ogg", false, false);
	//_loading->loadSound("ArrowImpact1", "./SOUND/PLAYER/ArrowImpact1.ogg", false, false);
	//_loading->loadSound("MinorArrowImpact1", "./SOUND/PLAYER/MinorArrowImpact1.ogg", false, false);

	//_loading->loadSound("Roll", "./SOUND/PLAYER/Roll.ogg", false, false);

	////eyedoor
	//_loading->loadSound("EnterDoor", "./SOUND/PLAYER/EnterDoor.ogg", false, false);
	//_loading->loadSound("EyeDoor", "./SOUND/PLAYER/EyeDoor.ogg", false, false);
	//_loading->loadSound("EyeDoorEyeOpen", "./SOUND/PLAYER/EyeDoorEyeOpen.ogg", false, false);
	//_loading->loadSound("BigDoor", "./SOUND/PLAYER/BigDoor.ogg", false, false);
	//
	////yeti
	//for (int i = 1; i < 13; i++)
	//{
	//	key = "IcicleLand";
	//	fileName = "./SOUND/SFX/BOSS/YETI/IcicleLand";
	//	keyTotal = key + to_string(i);
	//	fileNameTotal = fileName + to_string(i) + fileType;
	//	_loading->loadSound(keyTotal, fileNameTotal, false, false);
	//}
	//for (int i = 1; i < 11; i++)
	//{
	//	key = "IcicleSmash";
	//	fileName = "./SOUND/SFX/BOSS/YETI/IcicleSmash";
	//	keyTotal = key + to_string(i);
	//	fileNameTotal = fileName + to_string(i) + fileType;
	//	_loading->loadSound(keyTotal, fileNameTotal, false, false);
	//}
	//for (int i = 1; i < 5; i++)
	//{
	//	key = "Roll";
	//	fileName = "./SOUND/SFX/BOSS/YETI/Roll";
	//	keyTotal = key + to_string(i);
	//	fileNameTotal = fileName + to_string(i) + fileType;
	//	_loading->loadSound(keyTotal, fileNameTotal, false, false);

	//	key = "SnowImpact";
	//	fileName = "./SOUND/SFX/BOSS/YETI/SnowImpact";
	//	keyTotal = key + to_string(i);
	//	fileNameTotal = fileName + to_string(i) + fileType;
	//	_loading->loadSound(keyTotal, fileNameTotal, false, false);

	//	key = "SnowThrow";
	//	fileName = "./SOUND/SFX/BOSS/YETI/SnowThrow";
	//	keyTotal = key + to_string(i);
	//	fileNameTotal = fileName + to_string(i) + fileType;
	//	_loading->loadSound(keyTotal, fileNameTotal, false, false);

	//	key = "YetiBallImpact";
	//	fileName = "./SOUND/SFX/BOSS/YETI/YetiBallImpact";
	//	keyTotal = key + to_string(i);
	//	fileNameTotal = fileName + to_string(i) + fileType;
	//	_loading->loadSound(keyTotal, fileNameTotal, false, false);

	//	key = "YetiLand";
	//	fileName = "./SOUND/SFX/BOSS/YETI/YetiLand";
	//	keyTotal = key + to_string(i);
	//	fileNameTotal = fileName + to_string(i) + fileType;
	//	_loading->loadSound(keyTotal, fileNameTotal, false, false);
	//}
	//_loading->loadSound("Ascension", "./SOUND/SFX/TITANKILL/Ascension.ogg", false, false);
	//_loading->loadSound("Impact", "./SOUND/SFX/TITANKILL/Impact.ogg", false, false);
	//_loading->loadSound("Credits", "./SOUND/MUSIC/WORLD/Credits.ogg", true, true);

}

LoadingScene::LoadingScene()
{
}


LoadingScene::~LoadingScene()
{
}
