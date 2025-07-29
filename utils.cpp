#include "const_global.h" 
#include "engine.h"
#include "utils.h"

#include <string>
#include <bits/stdc++.h>

int getEncoding(std::string name)
{
    //std::cout<<name<<std::endl;
    int res = -1;
    int ls = name.back()-'0';
    int mod =0;
    /*if(name.back()=='-'){
        mod=1;
        name=name.substr(0, name.size()-1);
    }*/
    if(ls>=2&&ls<=4){
        mod+=2*(ls-1);
        name=name.substr(0, name.size()-1);
    }
    if (name == "Apprentice")
        res = 2;
    else if (name == "Archer")
        res = 10;
    else if (name == "Axeman")
        res = 18;
    else if (name == "Banshee")
        res = 26;
    else if (name == "Bat")
        res = 34;
    else if (name == "Bishop")
        res = 42;
    else if (name == "Bomber")
        res = 50;
    else if (name == "Dryad")
        res = 58;
    else if (name == "Duelist")
        res = 66;
    else if (name == "**Duelist")
        res = 74;
    else if (name == "Fireball")
        res = 82;
    else if (name == "Frog")
        res = 90;
    else if (name == "FrostMage")
        res = 98;
    else if (name == "Ghost")
        res = 106;
    else if (name == "Guardian")
        res = 114;
    else if (name == "Hoplite")
        res = 122;
    else if (name == "Knight")
        res = 130;
    else if (name == "Militia")
        res = 138;
    else if (name == "Pawn")
        res = 146;
    else if (name == "Penguin")
        res = 154;
    else if (name == "PoisonMage")
        res = 162;
    else if (name == "Shieldsman")
        res = 170;
    else if (name == "Skeleton")
        res = 178;
    else if (name == "Slime")
        res = 186;
    else if (name == "Spearman")
        res = 194;
    else if (name == "Spider")
        res = 202;
    else if (name == "Swordsman")
        res = 210;
    else if (name == "Tiger")
        res = 218;
    else if (name == "Warrior")
        res = 226;
    else if (name == "WindMage")
        res = 234;
    else if (name == "Alchemist")
        res = 242;
    else if (name == "Arachnid")
        res = 250;
    else if (name == "Basilisk")
        res = 258;
    else if (name == "Berserker")
        res = 266;
    else if (name == "Crusader")
        res = 274;
    else if (name == "**Crusader")
        res = 282;
    else if (name == "Dove")
        res = 290;
    else if (name == "Drake")
        res = 298;
    else if (name == "Fencer")
        res = 306;
    else if (name == "**Fencer")
        res = 314;
    else if (name == "FireMage")
        res = 322;
    else if (name == "FrostMephit")
        res = 330;
    else if (name == "GiantSlime")
        res = 338;
    else if (name == "Gnome")
        res = 346;
    else if (name == "Hostage")
        res = 354;
    else if (name == "Hydromancer")
        res = 362;
    else if (name == "Legionary")
        res = 370;
    else if (name == "MageTower")
        res = 378;
    else if (name == "Necromancer")
        res = 386;
    else if (name == "Nexus")
        res = 394;
    else if (name == "Paladin")
        res = 402;
    else if (name == "Phantasm")
        res = 410;
    else if (name == "Pikeman")
        res = 418;
    else if (name == "Portal")
        res = 426;
    else if (name == "Prince")
        res = 434;
    else if (name == "Princess")
        res = 442;
    else if (name == "Pyromancer")
        res = 450;
    else if (name == "Ranger")
        res = 458;
    else if (name == "Rook")
        res = 466;
    else if (name == "Samurai")
        res = 474;
    else if (name == "Snake")
        res = 482;
    else if (name == "StoneMage")
        res = 490;
    else if (name == "Sylph")
        res = 498;
    else if (name == "Templar")
        res = 506;
    else if (name == "ThunderMage")
        res = 514;
    else if (name == "Toad")
        res = 522;
    else if (name == "Wisp")
        res = 530;
    else if (name == "AirElemental")
        res = 538;
    else if (name == "ArchBishop")
        res = 546;
    else if (name == "Beacon")
        res = 554;
    else if (name == "Behemoth")
        res = 562;
    else if (name == "Butterfly")
        res = 570;
    else if (name == "Chastity")
        res = 578;
    else if (name == "Demon")
        res = 586;
    else if (name == "Dragon")
        res = 594;
    else if (name == "EarthElemental")
        res = 602;
    else if (name == "Ghast")
        res = 610;
    else if (name == "Harpy")
        res = 618;
    else if (name == "HauntedArmor")
        res = 626;
    else if (name == "**HauntedArmor")
        res = 634;
    else if (name == "LifeStone")
        res = 642;
    else if (name == "Medusa")
        res = 650;
    else if (name == "Mercenary")
        res = 658;
    else if (name == "Minotaur")
        res = 666;
    else if (name == "Ninja")
        res = 674;
    else if (name == "NullMage")
        res = 682;
    else if (name == "Patience")
        res = 690;
    else if (name == "Phalanx")
        res = 698;
    else if (name == "**Phalanx")
        res = 706;
    else if (name == "Queen")
        res = 714;
    else if (name == "Reaver")
        res = 722;
    else if (name == "Salamander")
        res = 730;
    else if (name == "SoulFlare")
        res = 738;
    else if (name == "Summoner")
        res = 746;
    else if (name == "Temperance")
        res = 754;
    else if (name == "Tombstone")
        res = 762;
    else if (name == "Undine")
        res = 770;
    else if (name == "Valkyrie")
        res = 778;
    else if (name == "Wizard")
        res = 786;
    else if (name == "Angel")
        res = 794;
    else if (name == "Antimage")
        res = 802;
    else if (name == "Aquarius")
        res = 810;
    else if (name == "Comet")
        res = 818;
    else if (name == "Enchantress")
        res = 826;
    else if (name == "Envy")
        res = 834;
    else if (name == "FireElemental")
        res = 842;
    else if (name == "Fortress")
        res = 850;
    else if (name == "Gemini")
        res = 858;
    else if (name == "GeminiTwin")
        res = 866;
    else if (name == "Gluttony")
        res = 874;
    else if (name == "GravityMage")
        res = 882;
    else if (name == "Greed")
        res = 890;
    else if (name == "Illusionist")
        res = 898;
    else if (name == "Illusion")
        res = 906;
    else if (name == "Lich")
        res = 914;
    else if (name == "Lilith")
        res = 922;
    else if (name == "Lust")
        res = 930;
    else if (name == "MoonFox")
        res = 938;
    else if (name == "Phoenix")
        res = 946;
    else if (name == "PheonixEgg")
        res = 954;
    else if (name == "Pride")
        res = 962;
    else if (name == "RoyalGuard")
        res = 970;
    else if (name == "Siren")
        res = 978;
    else if (name == "SoulKeeper")
        res = 986;
    else if (name == "Taurus")
        res = 994;
    else if (name == "Vampire")
        res = 1002;
    else if (name == "VoidMage")
        res = 1010;
    else if (name == "WaterElemental")
        res = 1018;
    else if (name == "Wrath")
        res = 1026;
    else if (name == "King")
        res = 1034;
    else if (name == "Sapling")
        res = 1036;
    else if(name == "Tree")
        res = 1038;
    else if(name == "BonePile")
        res = 1040;
    else if(name == "StonePillar")
        res = 1042;
    else
        throw std::invalid_argument( name+" not recognized");

    return res+mod;
}

void summonPiece(int x, int y, int t);

void loadPosition(std::string in)
{
    std::vector<std::string> res;
    int pos = 0;
    while (true)
    {
        pos = in.find(",");
        if(pos==std::string::npos)
            break;
        res.push_back(in.substr(0, pos));
        in.erase(0, pos + 1);
    }
    res.push_back(in);
    int version = res[0]=="56" ? 56 : res[0]=="57" ? 57 : 54;
    // start at the 38th slice
    int begin = version==56 ? 37 : 37;
    for(int i=37; i<37+64; i++){
        if(res[i]=="")
            continue;
        int x = (i-37)/8;
        int y = 7-(i-37)%8;
        //std::cout<<res[i]<<std::endl;
        int type = getEncoding(res[i])+(res[i+64][0]-'0');
        summonPiece(x, y, type);

        if(pieces[pc_cnt-1]==1034){
            king_id[0]=pc_cnt-1;
        }
        if(pieces[pc_cnt-1]==1035){
            king_id[1]=pc_cnt-1;
        }
    }
    undo_pnt[turn]=0;
}


std::string names[134] = {"Apprentice","Archer","Axeman","Banshee","Bat","Bishop","Bomber","Dryad","Duelist","**Duelist","FireBall","Frog","FrostMage","Ghost","Guardian","Hoplite","Knight","Militia","Pawn","Penguin","PoisonMage","Shieldsman","Skeleton","Slime","Spearman","Spider","Swordsman","Tiger","Warrior","WindMage","Alchemist","Arachnid","Basilisk","Berserker","Crusader","**Crusader","Dove","Drake","Fencer","**Fencer","FireMage","FrostMephit","GiantSlime","Gnome","Hostage","Hydromancer","Legionary","MageTower","Necromancer","Nexus","Paladin","Phantasm","Pikeman","Portal","Prince","Princess","Pyromancer","Ranger","Rook","Samurai","Snake","StoneMage","Sylph","Templar","ThunderMage","Toad","Wisp","AirElemental","ArchBishop","Beacon","Behemoth","Butterfly","Chastity","Demon","Dragon","EarthElemental","Ghast","Harpy","HauntedArmor","**HauntedArmor","LifeStone","Medusa","Mercenary","Minotaur","Ninja","NullMage","Patience","Phalanx","**Phalanx","Queen","Reaver","Salamander","SoulFlare","Summoner","Temperance","Tombstone","Undine","Valkyrie","Wizard","Angel","Antimage","Aquarius","Comet","Enchantress","Envy","FireElemental","Fortress","Gemini","GeminiTwin","Gluttony","GravityMage","Greed","Illusionist","Illusion","Lich","Lilith","Lust","MoonFox","Phoenix","PheonixEgg","Pride","RoyalGuard","Siren","SoulKeeper","Taurus","Vampire","VoidMage","WaterElemental","Wrath","King", "Sapling", "Tree", "BonePile", "StonePillar"};
std::string getName(int piece){
    bool black = (piece&1)==1;
    if((piece&1)==1)
        piece--;
    piece-=2;
    std::string name = names[piece/8];
    int mod =(piece%8)/2;
    std::string end = (black) ? "-" : "";
    if(mod==0){
        return name+end;
    }
    return name + std::to_string(mod+1) + end;
}

void recalculate_evaluation(){
    int white_position = 0;
    int black_position = 0;
    for(int x=0; x<8; x++)
        for(int y=0; y<8; y++){
            if(board[x][y]==0)
                continue;
            if((board[x][y]&1)==0)
                white_position+=piece_square_tables[piece_type[board[x][y]/2]][x][y];
            else
                black_position+=piece_square_tables[piece_type[board[x][y]/2]][x][y];
        }
    if(position_bonus[0]!=white_position||position_bonus[1]!=black_position){
        std::cout<<position_bonus[0]<<" "<<white_position<<" "<<position_bonus[1]<<" "<<black_position<<std::endl;
        throw std::logic_error("evaluation does not match");
    }
}

void check_state(){
    for(int i=0; i<pc_cnt; i++){
        if(death[i])
            continue;
        if(board[px[i]][py[i]]!=pieces[i]||id_board[px[i]][py[i]]!=i)
            std::cout<<"FAIL "<<i<<" "<<px[i]<<" "<<py[i]<<" "<<pieces[i]<<" "<<death[i]<<" "<<nodes<<std::endl;
    }
}

void printState(){

    std::cout<<"White morale: "<<morale[0]<<" "<<"Black morale: "<<morale[1]<<std::endl;
    std::cout<<static_evaluation(turn&1)<<std::endl;
    std::cout<<"Hash: "<<hash<<std::endl;
    for(int y=7; y>=0; y--){
        for(int x=0; x<=7; x++){
            if(board[x][y]==0)
                std::cout<<"-------("<<x<<","<<y<<")------- ";
            else{
                std::cout<<getName(board[x][y])<<"("<<id_board[x][y]<<") ";//["<<pmorale[id_board[x][y]]<<"]{"<<piece_type[board[x][y]/2]<<": "<<piece_square_tables[piece_type[board[x][y]/2]][board[x][y]&1 ? y : 7-y][x]<<"} ";
                if(getName(board[x][y]).size()<16)
                    std::cout<<std::string(16-getName(board[x][y]).size()-(id_board[x][y]>=10 ? 1 : 0), ' ');
            }
        }
        std::cout<<std::endl;
    }

    for(int i=0; i<pc_cnt; i++)
        std::cout<<"("<<i<<": "<<pmorale[i]<<", "<<status[i]<<", "<<death[i]<<", "<<nulled[i]<<") ";
    std::cout<<std::endl;
    check_state();
    std::cout<<std::endl;

}

void printCandidateMoves(){
    for(int i=0; i<candidate_pointer[turn]; i++)
        std::cout<<candidateMoveStack[turn][i][0]<<" "<<candidateMoveStack[turn][i][1]<<" "<<candidateMoveStack[turn][i][2]<<" "<<candidateMoveStack[turn][i][3]<<std::endl;
    std::cout << std::endl;
}

void printChosenMove(){
    std::cout<<"Best move: "<<move_chosen[0]<<" "<<move_chosen[1]<<" "<<move_chosen[2]<<" "<<move_chosen[3]<<std::endl;
}