#include <string>
#include "harc.h"
#include "level.h"
#include "globals.h"

using namespace globals;
using namespace std;

//TODO ha enemynek van shieldje, akkor ahhoz képest védekezzen: normál shield: 2 helyen, nagy shield: közép + 1 hely, az lesz a könyebb
//TODO fegyverekhez (+nagyshieldhez) különleges támadás, effektek, effektekhez meg eleve az egész effektek részt meg kell írni
//Ezt úgy, hogy fegyvertípust azonosítani, hozzáadni a különleges támadás nevét az attack choices-hoz, ehhez tárolni kell globálisan a neveket

std::string WeaponArtName;

void SetSpecial() {
	WeaponArtName = "";
	if (p1.wearing.weapon.wtype.find("sword",0)) { WeaponArtName = "Double Slash"; }
	if (p1.wearing.weapon.wtype.find("club",0)) { WeaponArtName = "Double Slash"; }
	if (p1.wearing.weapon.wtype.find("hammer",0)) { WeaponArtName = "Double Slash"; }
	if (p1.wearing.weapon.wtype.find("rapier",0)) { WeaponArtName = "Double Slash"; }
	if (p1.wearing.weapon.wtype.find("knife",0)) { WeaponArtName = "Double Slash"; }
	attack_choices.push_back(WeaponArtName);
	if (WeaponArtName!=""){ globals::menu_attack_size++; }
}

void PlayerAttack(enemy &e,int selIndx,std::vector<CONLOG> &con_log) {
    /*p1.subStamina(p1.wsc()/2);
    int dice=rand()%100;
    ADIR local_def=e.def;
    if(e.def!=ANONE) {
        if(dice>=60) local_def=(ADIR)(local_def+1);
        if(dice>=70) local_def=(ADIR)(local_def+1);
        if(dice>=80) local_def=(ADIR)(local_def+1);
        if(dice>=90) local_def=(ADIR)(local_def+1);
        local_def=(ADIR)(local_def%5);
    } else {
        local_def=(ADIR)0;
        if(dice>=20) local_def=(ADIR)(local_def+1);
        if(dice>=40) local_def=(ADIR)(local_def+1);
        if(dice>=60) local_def=(ADIR)(local_def+1);
        if(dice>=80) local_def=(ADIR)(local_def+1);
        local_def=(ADIR)(local_def%5);
    }
	int DDamage=0;
	e.wearing.shield.SubShieldDur(DDamage);
    if     (attack_choices[selIndx]=="from left"&&local_def==ALEFT&&e.getStamina()>=e.ssc()) {
		con_log.push_back( {"You attacked from left, but it got blocked.",TCOD_cyan}); e.subStamina(e.ssc()); }
    else if(attack_choices[selIndx]=="from above"&&local_def==AUP&&e.getStamina()>= e.ssc()) {
		con_log.push_back( {"You attacked from above, but it got blocked.",TCOD_cyan}); e.subStamina(e.ssc()); }
    else if(attack_choices[selIndx]=="from right"&&local_def==ARIGHT&&e.getStamina()>= e.ssc()) {
		con_log.push_back( {"You attacked from right, but it got blocked.",TCOD_cyan}); e.subStamina(e.ssc()); }
    else if(attack_choices[selIndx]=="from below"&&local_def==ADOWN&&e.getStamina()>= e.ssc()) {
		con_log.push_back( {"You attacked from below, but it got blocked.",TCOD_cyan}); e.subStamina(e.ssc()); }
    else if(attack_choices[selIndx]=="frontal attack"&&local_def==AMID&&e.getStamina()>= e.ssc()) {
		con_log.push_back( {"Your frontal attack got blocked.",TCOD_cyan}); e.subStamina(e.ssc()); }
    else {
		e.wearing.shield.SubShieldDur(-DDamage);
        p1.subStamina(p1.wsc()/2);
		switch (selIndx)
		{
		case 0:
		case 2:
			e.wearing.gloves.SubGlovesDur(DDamage / 2);
			e.wearing.chestpiece.SubArmorDur(DDamage / 2);
			break;
		case 1:
			e.wearing.headpiece.SubHeadpieceDur(DDamage);
			break;
		case 3:
			e.wearing.pants.SubPantsDur(DDamage);
			break;
		case 4:
			e.wearing.chestpiece.SubArmorDur(DDamage);
			break;
		}
        bool Critical=e.damage(p1.wearing.weapon.dmgx);
        con_log.push_back( {"Your attack was successful.",TCOD_pink});
        if(Critical) con_log.push_back( {"Critical damage!",TCOD_flame});
    }
	PrintPlayerStats();*/
}

void EnemyAttack(enemy &e,std::vector<CONLOG> &con_log) {
 //   e.subStamina(e.wsc()/2);
 //   int dice=rand()%100;
 //   ADIR local_atc=e.atc;
 //   if(e.atc!=ANONE) {
 //       if(dice>=60) local_atc=(ADIR)(local_atc+1);
 //       if(dice>=70) local_atc=(ADIR)(local_atc+1);
 //       if(dice>=80) local_atc=(ADIR)(local_atc+1);
 //       if(dice>=90) local_atc=(ADIR)(local_atc+1);
 //       local_atc=(ADIR)(local_atc%5);
 //   } else {
 //       local_atc=(ADIR)0;
 //       if(dice>=20) local_atc=(ADIR)(local_atc+1);
 //       if(dice>=40) local_atc=(ADIR)(local_atc+1);
 //       if(dice>=60) local_atc=(ADIR)(local_atc+1);
 //       if(dice>=80) local_atc=(ADIR)(local_atc+1);
 //       local_atc=(ADIR)(local_atc%5);
 //   }
 //   int selIndx=0;
 //   TCOD_key_t input;
 //   TCODConsole::root->rect(0,ConsoleHeight-menu_height,menu_ch_sel_width,menu_height,true);
 //   if(p1.getStamina()>= p1.ssc()) {
 //       for(int i=0; i<menu_defense_size; i++) TCODConsole::root->print(0,ConsoleHeight-menu_height+i,"%s",defense_choices[i].c_str());
 //       do {
 //           MenuSelection(0,selIndx,defense_choices,input,con_log);
 //           if(input.vk==TCODK_UP&&selIndx-1>=0) selIndx--;
 //           if(input.vk==TCODK_DOWN&&selIndx+1<=menu_defense_size-1) selIndx++;
 //       } while(input.vk!=TCODK_ENTER&&input.vk!=TCODK_ESCAPE&&!TCODConsole::isWindowClosed());
 //   } else {
 //       do {
 //           TCODConsole::root->print(0,ConsoleHeight-menu_height,"You are too tired to block.");
 //           TCOD_console_flush();
 //           input=TCODConsole::waitForKeypress(true); //Waiting for keypress
 //       } while(input.vk!=TCODK_ENTER&&input.vk!=TCODK_ESCAPE&&!TCODConsole::isWindowClosed());
 //   }
 //   TCODConsole::root->rect(0,ConsoleHeight-menu_height,menu_ch_sel_width*2,menu_height,true);
	//int DDamage = e.wearing.weapon.durdmg;
	//p1.wearing.shield.SubShieldDur(DDamage);
 //   if     (defense_choices[selIndx]=="block from left"&&local_atc==ALEFT&&p1.getStamina()>= p1.ssc()) {
	//	con_log.push_back({ "You successfuly blocked an attack from left!",TCOD_gold });
	//	p1.subStamina(p1.ssc()); p1.wearing.shield.durability -= e.wearing.weapon.durdmg; }
 //   else if(defense_choices[selIndx]=="block from above"&&local_atc==AUP&&p1.getStamina()>= p1.ssc()) {
	//	con_log.push_back( {"You successfuly blocked an attack from above!",TCOD_gold});
	//	p1.subStamina(p1.ssc()); p1.wearing.shield.durability -= e.wearing.weapon.durdmg; }
 //   else if(defense_choices[selIndx]=="block from right"&&local_atc==ARIGHT&&p1.getStamina()>= p1.ssc()) {
	//	con_log.push_back( {"You successfuly blocked an attack from right!",TCOD_gold});
	//	p1.subStamina(p1.ssc()); p1.wearing.shield.durability -= e.wearing.weapon.durdmg; }
 //   else if(defense_choices[selIndx]=="block from below"&&local_atc==ADOWN&&p1.getStamina()>= p1.ssc()) {
	//	con_log.push_back( {"You successfuly blocked an attack from below!",TCOD_gold});
	//	p1.subStamina(p1.ssc()); p1.wearing.shield.durability -= e.wearing.weapon.durdmg; }
 //   else if(defense_choices[selIndx]=="block frontal attack"&&local_atc==AMID&&p1.getStamina()>= p1.ssc()) {
	//	con_log.push_back( {"You successfuly blocked a frontal attack!",TCOD_gold});
	//	p1.subStamina(p1.ssc()); p1.wearing.shield.durability -= e.wearing.weapon.durdmg; }
 //   else {
 //       e.subStamina(e.wsc()/2);
	//	p1.wearing.shield.SubShieldDur(-DDamage);
 //       bool Critical=p1.damage(e.wearing.weapon.dmgx * e.offense);
	//	if (local_atc == ALEFT)			{
	//		con_log.push_back({ "The enemy successfuly hit you from left.",TCOD_purple });
	//		p1.wearing.gloves.SubGlovesDur(DDamage / 2);
	//		p1.wearing.chestpiece.SubArmorDur(DDamage / 2);
	//	}
	//	else if (local_atc == AUP)		{
	//		con_log.push_back({ "The enemy successfuly hit you from above.",TCOD_purple });
	//		e.wearing.headpiece.SubHeadpieceDur(DDamage);
	//	}
	//	else if (local_atc == ARIGHT)	{
	//		con_log.push_back({ "The enemy successfuly hit you from right.",TCOD_purple });
	//		e.wearing.gloves.SubGlovesDur(DDamage / 2);
	//		e.wearing.chestpiece.SubArmorDur(DDamage / 2);
	//	}
	//	else if (local_atc == ADOWN)	{
	//		con_log.push_back({ "The enemy successfuly hit you from below.",TCOD_purple });
	//		e.wearing.pants.SubPantsDur(DDamage);
	//	}
	//	else if (local_atc == AMID)		{
	//		con_log.push_back({ "The enemy successfuly hit you with a frontal attack.",TCOD_purple });
	//		e.wearing.chestpiece.SubArmorDur(DDamage);
	//	}
 //       if(Critical) con_log.push_back( {"Critical damage!",TCOD_flame});
 //   }
	//PrintPlayerStats();
}

void PrintPlayerStats() {
    /*TCODConsole::root->rect(0,0,18,2,true);
    TCODConsole::root->setDefaultForeground(TCOD_red);
    TCODConsole::root->print(0,0,"     HP: %3d%%/100%%",p1.getHP());
    TCODConsole::root->setDefaultForeground(TCOD_green);
    TCODConsole::root->print(0,1,"STAMINA: %3d%%/100%%",p1.getStamina());
    TCODConsole::root->setDefaultForeground(TCOD_white);*/
}

//void MenuSelection(int StartingPos,int selIndx,std::string choices[],TCOD_key_t&input,std::vector<CONLOG> &con_log) {
    //Display log
    //TCODConsole::root->setDefaultBackground(TCOD_darkest_gray);
    //TCODConsole::root->setDefaultForeground(TCOD_white);
    //TCODConsole::root->rect(con_log_StartingPos,ConsoleHeight-menu_height,ConsoleWidth-con_log_StartingPos,menu_height,true,TCOD_BKGND_SET);
    //for(int i=con_log.size()-menu_height; i<(int)con_log.size(); i++) {
    //    if(i>=0) {
    //        TCODConsole::root->setDefaultForeground(con_log[i].color);
    //        TCODConsole::root->print(con_log_StartingPos,ConsoleHeight+i-con_log.size(),"%s",con_log[i].msg.c_str());
    //    }
    //}

    //TCODConsole::root->setDefaultBackground(TCOD_white);
    //TCODConsole::root->setDefaultForeground(TCOD_black);
    //TCODConsole::root->print(StartingPos,ConsoleHeight-menu_height+selIndx,"%s",choices[selIndx].c_str());
    //TCODConsole::root->rect(StartingPos,ConsoleHeight-menu_height+selIndx,menu_ch_sel_width,1,false,TCOD_BKGND_SET);
    //TCOD_console_flush();
    //input=TCODConsole::waitForKeypress(true); //Waiting for keypress
    //TCODConsole::root->setDefaultBackground(TCOD_black);
    //TCODConsole::root->setDefaultForeground(TCOD_white);
    //TCODConsole::root->print(StartingPos,ConsoleHeight-menu_height+selIndx,"%s",choices[selIndx].c_str());
    //TCODConsole::root->rect(StartingPos,ConsoleHeight-menu_height+selIndx,menu_ch_sel_width,1,false,TCOD_BKGND_SET);
//}

void HarcGUI(enemy e,ROUND most) {
    /*std::vector<CONLOG> con_log;
    con_log.push_back( {"A wild "+e.name+" appeared.",TCOD_white});

    TCODConsole::root->clear();

    WriteOutPic(&e.PNG, enemy::TopLeft, enemy::BoxSize);

    int selIndx=0;
    int sub_selIndx=0;
    TCOD_key_t input;
    do {
        PrintPlayerStats();
		if (most == PLAYER_KOR) {
			attack_choices = { "from left","from above","from right","from below","frontal attack" };
			globals::menu_attack_size = 5;
			SetSpecial();
			for (int i = 0; i < menu_harc_size; i++) TCODConsole::root->print(0, ConsoleHeight - menu_height + i, "%s", harc_menu_choices[i].c_str());
			MenuSelection(0, selIndx, harc_menu_choices, input, con_log);
			if (input.vk == TCODK_UP&&selIndx - 1 >= 0) selIndx--;
			if (input.vk == TCODK_DOWN&&selIndx + 1 <= menu_harc_size - 1) selIndx++;
			if (input.vk == TCODK_ENTER) {
				if (harc_menu_choices[selIndx] == "Attack"&&p1.getStamina() >= 10) {
					for (int i = 0; i < menu_attack_size; i++) TCODConsole::root->print(menu_ch_sel_width, ConsoleHeight - menu_height + i, "%s", attack_choices[i].c_str());
					do {
						MenuSelection(menu_ch_sel_width, sub_selIndx, attack_choices.data(), input, con_log);
						if (input.vk == TCODK_UP&&sub_selIndx - 1 >= 0) sub_selIndx--;
						if (input.vk == TCODK_DOWN&&sub_selIndx + 1 <= menu_attack_size - 1) sub_selIndx++;
					} while (input.vk != TCODK_ENTER&&input.vk != TCODK_ESCAPE&&!TCODConsole::isWindowClosed());
					PlayerAttack(e, sub_selIndx, con_log);
					most = ENEMY_KOR;
					sub_selIndx = 0;
					TCODConsole::root->rect(menu_ch_sel_width, ConsoleHeight - menu_height, menu_ch_sel_width, menu_height, true);
				} else if (harc_menu_choices[selIndx] == "Attack") con_log.push_back({ "You are exhausted, you can't attack.",TCOD_white });
				if (harc_menu_choices[selIndx] == "Use item") {
					for (int i = 0; i < menu_useitem_size; i++) TCODConsole::root->print(menu_ch_sel_width, ConsoleHeight - menu_height + i, "%s", useitem_choices[i].c_str());
					do {
						MenuSelection(menu_ch_sel_width, sub_selIndx, useitem_choices, input, con_log);
						if (input.vk == TCODK_UP&&sub_selIndx - 1 >= 0) sub_selIndx--;
						if (input.vk == TCODK_DOWN&&sub_selIndx + 1 <= menu_useitem_size - 1) sub_selIndx++;
					} while (input.vk != TCODK_ENTER&&input.vk != TCODK_ESCAPE&&!TCODConsole::isWindowClosed());
					most = ENEMY_KOR;
					sub_selIndx = 0;
					TCODConsole::root->rect(menu_ch_sel_width, ConsoleHeight - menu_height, menu_ch_sel_width, menu_height, true);
				}
				if (harc_menu_choices[selIndx] == "Rest") {
					p1.addStamina(50);
					con_log.push_back({ "You gathered your strength.",TCOD_green });
					PrintPlayerStats();
					most = ENEMY_KOR;
				}
			}
		}
        if(most==ENEMY_KOR) {
        switch(e.getHP()/10) {
        case 0:
            con_log.push_back( {e.name+" is dying.",TCOD_red});
            break;
        case 1:
            con_log.push_back( {e.name+" writhes in agony.",TCOD_red});
            break;
        case 2:
            con_log.push_back( {"Where does all this blood come from?",TCOD_red});
            break;
        case 3:
            con_log.push_back( {e.name+" is bleeding badly AND screams.",TCOD_light_red});
            break;
        case 4:
            con_log.push_back( {e.name+" is bleeding badly.",TCOD_light_red});
            break;
        case 5:
            con_log.push_back( {e.name+" screams.",TCOD_light_red});
            break;
        case 6:
            con_log.push_back( {e.name+" is bleeding.",TCOD_lighter_red});
            break;
        case 7:
            con_log.push_back( {e.name+" is getting hurt.",TCOD_lighter_red});
            break;
        case 8:
            con_log.push_back( {"Looks like "+e.name+" can bleed.",TCOD_lighter_red});
            break;
        case 9:
            con_log.push_back( {"It seems "+e.name+" can take damage.",TCOD_lightest_red});
            break;
        }
        if     (e.getStamina()<10) con_log.push_back( {e.name+" looks exhausted.",TCOD_light_green});
        else if(e.getStamina()<20) con_log.push_back( {e.name+" is getting tired.",TCOD_light_green});

        if(e.getStamina()<10) {
            con_log.push_back( {e.name+" gathers his stenght.",TCOD_green});
            e.addStamina(50);
        } else EnemyAttack(e,con_log);
            most=PLAYER_KOR;
        }
    } while(e.getHP()>0&&p1.getHP()>0&&input.vk!=TCODK_ESCAPE&&!TCODConsole::isWindowClosed());
    if(e.getHP()==0) {
        con_log.push_back( {e.name+" is dead!",TCOD_red});
        std::string anyad[]={"Victory!"};
        MenuSelection(0,0,anyad,input,con_log);
    }
    if(p1.getHP()==0) {
        con_log.push_back( {"K den.",TCOD_red});
        std::string anyad[]={"Game Over!"};
        MenuSelection(0,0,anyad,input,con_log);
    }
    p1.addHP(100);
    p1.addStamina(100);*/
}
