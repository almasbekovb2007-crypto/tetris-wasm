#include <emscripten.h>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cmath>
#include <vector>
#include <memory>

// ============================================================
//  АБСТРАКТНЫЙ БАЗОВЫЙ КЛАСС — GameObject
//  Демонстрирует: абстракцию + полиморфизм
// ============================================================
class GameObject {
public:
    virtual void update() = 0;
    virtual void reset()  = 0;
    virtual ~GameObject() {}
};

// ============================================================
//  АБСТРАКТНЫЙ КЛАСС — Piece
//  Демонстрирует: инкапсуляцию + абстракцию
// ============================================================
class Piece : public GameObject {
protected:
    int blocks[7][2];
    int size;
    int row, col, color;

public:
    Piece() : size(0), row(0), col(0), color(1) {
        memset(blocks, 0, sizeof(blocks));
    }
    virtual ~Piece() {}

    virtual void initShape() = 0;  // чисто виртуальный — каждый тип реализует свою форму
    void update() override {}
    void reset()  override { row = 0; }

    // Геттеры/сеттеры — инкапсуляция
    int getSize()  const { return size; }
    int getRow()   const { return row; }
    int getCol()   const { return col; }
    int getColor() const { return color; }
    int getBlockRow(int i) const { return row + blocks[i][0]; }
    int getBlockCol(int i) const { return col + blocks[i][1]; }
    void setRow(int r)   { row = r; }
    void setCol(int c)   { col = c; }
    void setColor(int c) { color = c; }
    void moveRow(int dr) { row += dr; }
    void moveCol(int dc) { col += dc; }

    void getBlocks(int out[7][2]) const { memcpy(out, blocks, sizeof(int)*size*2); }
    void setBlocks(int in[7][2], int sz) { memcpy(blocks, in, sizeof(int)*sz*2); size = sz; }
};

// ============================================================
//  КОНКРЕТНЫЕ КЛАССЫ ФИГУР
//  Демонстрирует: наследование + полиморфизм (override)
// ============================================================
class TetroPiece : public Piece {
public:
    void initShape() override {
        static int shapes[7][4][2] = {
            {{0,0},{0,1},{0,2},{0,3}},
            {{0,0},{0,1},{1,0},{1,1}},
            {{0,1},{1,0},{1,1},{1,2}},
            {{0,0},{1,0},{1,1},{1,2}},
            {{0,2},{1,0},{1,1},{1,2}},
            {{0,0},{0,1},{1,1},{1,2}},
            {{0,1},{0,2},{1,0},{1,1}},
        };
        int t = rand()%7; size = 4;
        for(int i=0;i<4;i++){blocks[i][0]=shapes[t][i][0];blocks[i][1]=shapes[t][i][1];}
        color=(rand()%7)+1;
    }
};

class PentoPiece : public Piece {
public:
    void initShape() override {
        static int shapes[12][5][2] = {
            {{0,0},{0,1},{0,2},{0,3},{0,4}},
            {{0,0},{0,1},{0,2},{0,3},{1,3}},
            {{0,0},{0,1},{0,2},{0,3},{1,0}},
            {{0,0},{0,1},{0,2},{1,1},{2,1}},
            {{0,0},{0,1},{1,1},{1,2},{2,2}},
            {{0,1},{0,2},{1,0},{1,1},{2,0}},
            {{0,0},{0,1},{0,2},{1,0},{1,1}},
            {{0,0},{0,1},{0,2},{1,1},{1,2}},
            {{0,0},{1,0},{1,1},{1,2},{2,2}},
            {{0,0},{0,1},{0,2},{1,2},{2,2}},
            {{0,0},{1,0},{2,0},{2,1},{2,2}},
            {{0,0},{0,1},{1,1},{2,1},{2,2}},
        };
        int t=rand()%12; size=5;
        for(int i=0;i<5;i++){blocks[i][0]=shapes[t][i][0];blocks[i][1]=shapes[t][i][1];}
        color=(rand()%7)+1;
    }
};

class HexaPiece : public Piece {
public:
    void initShape() override {
        static int shapes[10][6][2] = {
            {{0,0},{0,1},{0,2},{0,3},{0,4},{0,5}},
            {{0,0},{0,1},{0,2},{0,3},{0,4},{1,4}},
            {{0,0},{0,1},{0,2},{0,3},{1,0},{2,0}},
            {{0,0},{0,1},{0,2},{1,1},{1,2},{2,2}},
            {{0,0},{0,1},{1,1},{1,2},{2,2},{2,3}},
            {{0,0},{0,1},{0,2},{1,0},{1,1},{1,2}},
            {{0,1},{0,2},{1,0},{1,1},{2,0},{2,1}},
            {{0,0},{0,1},{0,2},{0,3},{1,1},{1,2}},
            {{0,0},{1,0},{1,1},{1,2},{2,2},{3,2}},
            {{0,0},{0,1},{1,1},{2,1},{2,2},{3,2}},
        };
        int t=rand()%10; size=6;
        for(int i=0;i<6;i++){blocks[i][0]=shapes[t][i][0];blocks[i][1]=shapes[t][i][1];}
        color=(rand()%7)+1;
    }
};

class HeptaPiece : public Piece {
public:
    void initShape() override {
        static int shapes[10][7][2] = {
            {{0,0},{0,1},{0,2},{0,3},{0,4},{0,5},{0,6}},
            {{0,0},{0,1},{0,2},{0,3},{0,4},{0,5},{1,5}},
            {{0,0},{0,1},{0,2},{0,3},{1,0},{1,1},{2,1}},
            {{0,0},{0,1},{0,2},{1,1},{1,2},{2,2},{2,3}},
            {{0,0},{0,1},{1,0},{1,1},{2,0},{2,1},{3,0}},
            {{0,0},{0,1},{0,2},{0,3},{1,0},{2,0},{3,0}},
            {{0,2},{0,3},{1,0},{1,1},{1,2},{2,0},{3,0}},
            {{0,0},{0,1},{0,2},{1,2},{1,3},{2,3},{2,4}},
            {{0,0},{1,0},{1,1},{2,1},{2,2},{3,2},{3,3}},
            {{0,0},{0,1},{0,2},{1,1},{2,1},{3,1},{3,2}},
        };
        int t=rand()%10; size=7;
        for(int i=0;i<7;i++){blocks[i][0]=shapes[t][i][0];blocks[i][1]=shapes[t][i][1];}
        color=(rand()%7)+1;
    }
};

// ============================================================
//  ФАБРИЧНЫЙ МЕТОД — PieceFactory
//  Демонстрирует: полиморфизм
// ============================================================
class PieceFactory {
public:
    static Piece* create(int pieceSize) {
        Piece* p = nullptr;
        switch(pieceSize) {
            case 5: p = new PentoPiece(); break;
            case 6: p = new HexaPiece();  break;
            case 7: p = new HeptaPiece(); break;
            default:p = new TetroPiece(); break;
        }
        p->initShape();
        return p;
    }
};

// ============================================================
//  КЛАСС — Field (игровое поле)
// ============================================================
#define MAX_W 30
#define MAX_H 40

class Field : public GameObject {
private:
    int grid[MAX_H][MAX_W];
    int ghost[MAX_H][MAX_W];
    int width, height;

public:
    Field(int w, int h) : width(w), height(h) { reset(); }
    void update() override {}
    void reset()  override { memset(grid,0,sizeof(grid)); memset(ghost,0,sizeof(ghost)); }

    int getWidth()  const { return width; }
    int getHeight() const { return height; }

    int getCell(int r, int c) const {
        if(r<0||r>=height||c<0||c>=width) return 0;
        if(grid[r][c])  return grid[r][c];
        if(ghost[r][c]) return -1;
        return 0;
    }

    bool canPlace(const Piece* p, int dr, int dc) const {
        int blk[7][2]; p->getBlocks(blk);
        for(int i=0;i<p->getSize();i++){
            int r=p->getRow()+blk[i][0]+dr;
            int c=p->getCol()+blk[i][1]+dc;
            if(r>=height) return false;
            if(c<0||c>=width) return false;
            if(r>=0&&grid[r][c]) return false;
        }
        return true;
    }

    void lockPiece(const Piece* p) {
        int blk[7][2]; p->getBlocks(blk);
        for(int i=0;i<p->getSize();i++){
            int r=p->getRow()+blk[i][0];
            int c=p->getCol()+blk[i][1];
            if(r>=0&&r<height&&c>=0&&c<width) grid[r][c]=p->getColor();
        }
    }

    int clearFullLines() {
        int cleared=0;
        for(int r=height-1;r>=0;r--){
            bool full=true;
            for(int c=0;c<width;c++) if(!grid[r][c]){full=false;break;}
            if(full){
                for(int rr=r;rr>0;rr--) memcpy(grid[rr],grid[rr-1],sizeof(int)*width);
                memset(grid[0],0,sizeof(int)*width);
                cleared++; r++;
            }
        }
        return cleared;
    }

    void updateGhost(const Piece* p) {
        memset(ghost,0,sizeof(ghost));
        int drop=0;
        while(canPlace(p,drop+1,0)) drop++;
        int blk[7][2]; p->getBlocks(blk);
        for(int i=0;i<p->getSize();i++){
            int r=p->getRow()+blk[i][0]+drop;
            int c=p->getCol()+blk[i][1];
            if(r>=0&&r<height&&c>=0&&c<width) ghost[r][c]=-1;
        }
    }
};

// ============================================================
//  КЛАСС — ScoreManager
// ============================================================
#define MAX_RECORDS 64
class ScoreManager : public GameObject {
private:
    char keys[MAX_RECORDS][16];
    int  vals[MAX_RECORDS];
    int  count;
public:
    ScoreManager():count(0){memset(keys,0,sizeof(keys));memset(vals,0,sizeof(vals));}
    void update() override {}
    void reset()  override { count=0; }
    int getRecord(const char* key) const {
        for(int i=0;i<count;i++) if(strcmp(keys[i],key)==0) return vals[i];
        return 0;
    }
    void setRecord(const char* key, int val) {
        for(int i=0;i<count;i++){
            if(strcmp(keys[i],key)==0){if(val>vals[i])vals[i]=val;return;}
        }
        if(count<MAX_RECORDS){strncpy(keys[count],key,15);vals[count]=val;count++;}
    }
    int getCount() const {return count;}
    const char* getKey(int i) const {return i<count?keys[i]:"";}
    int getVal(int i) const {return i<count?vals[i]:0;}
};

// ============================================================
//  КЛАСС — Settings
// ============================================================
class Settings : public GameObject {
private:
    int sound,music,dark,lefty,lang;
public:
    Settings():sound(1),music(1),dark(1),lefty(0),lang(0){}
    void update() override {}
    void reset()  override {sound=1;music=1;dark=1;lefty=0;lang=0;}
    int getSound()const{return sound;} void setSound(int v){sound=v;}
    int getMusic()const{return music;} void setMusic(int v){music=v;}
    int getDark() const{return dark;}  void setDark(int v) {dark=v;}
    int getLefty()const{return lefty;} void setLefty(int v){lefty=v;}
    int getLang() const{return lang;}  void setLang(int v) {lang=v;}
};

// ============================================================
//  ГЛАВНЫЙ КЛАСС — Game
//  Агрегирует объекты, полиморфный update через vector<GameObject*>
// ============================================================
class Game : public GameObject {
private:
    Field*        field;
    Piece*        curPiece;
    Piece*        nextPiece;
    ScoreManager* scores;
    Settings*     settings;
    std::vector<GameObject*> objects;  // полиморфизм

    int pieceSize, score, lines, level;
    bool running, paused, over;
    double lastDropTime, dropInterval;

    void calcInterval(){
        dropInterval=800.0*pow(0.85,level-1);
        if(dropInterval<80.0)dropInterval=80.0;
    }
    void centerPiece(Piece* p){
        int maxC=0; int blk[7][2]; p->getBlocks(blk);
        for(int i=0;i<p->getSize();i++) if(blk[i][1]>maxC)maxC=blk[i][1];
        p->setCol((field->getWidth()-maxC-1)/2);
        p->setRow(0);
    }
    void spawnNext(){
        delete curPiece;
        curPiece=nextPiece;
        nextPiece=PieceFactory::create(pieceSize);
        centerPiece(curPiece);
        field->updateGhost(curPiece);
    }

public:
    Game(int w, int h, int psz, ScoreManager* sm, Settings* st)
        :field(nullptr),curPiece(nullptr),nextPiece(nullptr),
         scores(sm),settings(st),pieceSize(psz),
         score(0),lines(0),level(1),
         running(false),paused(false),over(false),
         lastDropTime(0.0),dropInterval(800.0)
    {
        field=new Field(w,h);
        curPiece=PieceFactory::create(pieceSize);
        nextPiece=PieceFactory::create(pieceSize);
        centerPiece(curPiece);
        objects.push_back(field);  // полиморфный список
        field->updateGhost(curPiece);
        running=true; calcInterval();
    }
    ~Game(){delete field;delete curPiece;delete nextPiece;}

    // Полиморфный обход — демонстрация полиморфизма
    void update() override { for(auto o:objects) o->update(); }
    void reset()  override {
        score=0;lines=0;level=1;
        running=true;paused=false;over=false;lastDropTime=0.0;
        field->reset();
        delete curPiece;  curPiece=PieceFactory::create(pieceSize);
        delete nextPiece; nextPiece=PieceFactory::create(pieceSize);
        centerPiece(curPiece);
        field->updateGhost(curPiece);
        calcInterval();
    }

    int step(double nowMs){
        if(!running||paused||over) return 0;
        if(lastDropTime==0.0){lastDropTime=nowMs;return 0;}
        if(nowMs-lastDropTime<dropInterval) return 0;
        lastDropTime=nowMs;
        if(field->canPlace(curPiece,1,0)){
            curPiece->moveRow(1);
            field->updateGhost(curPiece);
            return 0;
        }
        field->lockPiece(curPiece);
        int cl=field->clearFullLines();
        lines+=cl; score+=curPiece->getSize();
        if(cl==1)score+=10;
        else if(cl==2)score+=25;
        else if(cl==3)score+=45;
        else if(cl>=4)score+=70*cl;
        level=1+lines/10; calcInterval();
        spawnNext();
        if(!field->canPlace(curPiece,0,0)){
            over=true;running=false;
            char key[16]; snprintf(key,16,"%dx%d",field->getWidth(),field->getHeight());
            scores->setRecord(key,score);
            return 2;
        }
        return 1;
    }

    void moveLeft(){
        if(!running||paused||over)return;
        if(field->canPlace(curPiece,0,-1)){curPiece->moveCol(-1);field->updateGhost(curPiece);}
    }
    void moveRight(){
        if(!running||paused||over)return;
        if(field->canPlace(curPiece,0,1)){curPiece->moveCol(1);field->updateGhost(curPiece);}
    }
    void moveDown(double nowMs){
        if(!running||paused||over)return;
        if(field->canPlace(curPiece,1,0)){
            curPiece->moveRow(1);
            lastDropTime = nowMs;
            field->updateGhost(curPiece);
        }
    }
    void hardDrop(){
        if(!running||paused||over)return;
        while(field->canPlace(curPiece,1,0))curPiece->moveRow(1);
        lastDropTime=0.0;
    }
    void rotate(){
        if(!running||paused||over)return;
        int sz=curPiece->getSize();
        int blk[7][2]; curPiece->getBlocks(blk);
        int maxR=0;
        for(int i=0;i<sz;i++) if(blk[i][0]>maxR)maxR=blk[i][0];
        int rot[7][2];
        for(int i=0;i<sz;i++){rot[i][0]=blk[i][1];rot[i][1]=maxR-blk[i][0];}
        int minR=99,minC=99;
        for(int i=0;i<sz;i++){if(rot[i][0]<minR)minR=rot[i][0];if(rot[i][1]<minC)minC=rot[i][1];}
        for(int i=0;i<sz;i++){rot[i][0]-=minR;rot[i][1]-=minC;}
        int kicks[5][2]={{0,0},{0,-1},{0,1},{0,-2},{0,2}};
        int origR=curPiece->getRow(),origC=curPiece->getCol();
        int origB[7][2]; curPiece->getBlocks(origB);
        for(int k=0;k<5;k++){
            curPiece->setBlocks(rot,sz);
            curPiece->setRow(origR+kicks[k][0]);
            curPiece->setCol(origC+kicks[k][1]);
            if(field->canPlace(curPiece,0,0)){field->updateGhost(curPiece);return;}
        }
        curPiece->setBlocks(origB,sz);
        curPiece->setRow(origR); curPiece->setCol(origC);
    }
    void togglePause(){if(!over){paused=!paused;if(!paused)lastDropTime=0.0;}}

    bool isRunning()const{return running;}
    bool isPaused() const{return paused;}
    bool isOver()   const{return over;}
    int  getScore() const{return score;}
    int  getLines() const{return lines;}
    int  getLevel() const{return level;}
    int  getGridW() const{return field->getWidth();}
    int  getGridH() const{return field->getHeight();}
    int  getBest()  const{
        char key[16]; snprintf(key,16,"%dx%d",field->getWidth(),field->getHeight());
        return scores->getRecord(key);
    }
    int getCell(int r,int c)       const{return field->getCell(r,c);}
    int getCurSize()               const{return curPiece->getSize();}
    int getCurColor()              const{return curPiece->getColor();}
    int getCurBlockR(int i)        const{return curPiece->getBlockRow(i);}
    int getCurBlockC(int i)        const{return curPiece->getBlockCol(i);}
    int getNextSize()              const{return nextPiece->getSize();}
    int getNextColor()             const{return nextPiece->getColor();}
    int getNextBlockR(int i) const { int blk[7][2]; nextPiece->getBlocks(blk); return blk[i][0]; }
    int getNextBlockC(int i) const { int blk[7][2]; nextPiece->getBlocks(blk); return blk[i][1]; }
};

// ============================================================
//  ГЛОБАЛЬНЫЕ ОБЪЕКТЫ
// ============================================================
static Game*         g_game    = nullptr;
static ScoreManager* g_scores  = nullptr;
static Settings*     g_settings= nullptr;

extern "C" {

EMSCRIPTEN_KEEPALIVE
void game_init(int w, int h, int psz) {
    srand((unsigned)time(nullptr));
    if(!g_scores)  g_scores  = new ScoreManager();
    if(!g_settings)g_settings= new Settings();
    delete g_game;
    g_game = new Game(w,h,psz,g_scores,g_settings);
}

EMSCRIPTEN_KEEPALIVE int  game_update(double t)  { return g_game?g_game->step(t):0; }
EMSCRIPTEN_KEEPALIVE void game_restart()          { if(g_game)g_game->reset(); }
EMSCRIPTEN_KEEPALIVE void action_left()           { if(g_game)g_game->moveLeft(); }
EMSCRIPTEN_KEEPALIVE void action_right()          { if(g_game)g_game->moveRight(); }
EMSCRIPTEN_KEEPALIVE void action_down(double t)    { if(g_game)g_game->moveDown(t); }
EMSCRIPTEN_KEEPALIVE void action_hard_drop()      { if(g_game)g_game->hardDrop(); }
EMSCRIPTEN_KEEPALIVE void action_rotate()         { if(g_game)g_game->rotate(); }
EMSCRIPTEN_KEEPALIVE void action_pause()          { if(g_game)g_game->togglePause(); }

EMSCRIPTEN_KEEPALIVE int get_grid_w()      {return g_game?g_game->getGridW():10;}
EMSCRIPTEN_KEEPALIVE int get_grid_h()      {return g_game?g_game->getGridH():20;}
EMSCRIPTEN_KEEPALIVE int get_score()       {return g_game?g_game->getScore():0;}
EMSCRIPTEN_KEEPALIVE int get_lines()       {return g_game?g_game->getLines():0;}
EMSCRIPTEN_KEEPALIVE int get_level()       {return g_game?g_game->getLevel():1;}
EMSCRIPTEN_KEEPALIVE int get_best()        {return g_game?g_game->getBest():0;}
EMSCRIPTEN_KEEPALIVE int get_game_over()   {return g_game?g_game->isOver():0;}
EMSCRIPTEN_KEEPALIVE int get_game_paused() {return g_game?g_game->isPaused():0;}
EMSCRIPTEN_KEEPALIVE int get_running()     {return g_game?g_game->isRunning():0;}
EMSCRIPTEN_KEEPALIVE int get_cell(int r,int c){return g_game?g_game->getCell(r,c):0;}

EMSCRIPTEN_KEEPALIVE int get_cur_size()        {return g_game?g_game->getCurSize():0;}
EMSCRIPTEN_KEEPALIVE int get_cur_color()       {return g_game?g_game->getCurColor():0;}
EMSCRIPTEN_KEEPALIVE int get_cur_block_r(int i){return g_game?g_game->getCurBlockR(i):0;}
EMSCRIPTEN_KEEPALIVE int get_cur_block_c(int i){return g_game?g_game->getCurBlockC(i):0;}
EMSCRIPTEN_KEEPALIVE int get_next_size()       {return g_game?g_game->getNextSize():0;}
EMSCRIPTEN_KEEPALIVE int get_next_color()      {return g_game?g_game->getNextColor():0;}
EMSCRIPTEN_KEEPALIVE int get_next_block_r(int i){return g_game?g_game->getNextBlockR(i):0;}
EMSCRIPTEN_KEEPALIVE int get_next_block_c(int i){return g_game?g_game->getNextBlockC(i):0;}

EMSCRIPTEN_KEEPALIVE void set_sound(int v){if(g_settings)g_settings->setSound(v);}
EMSCRIPTEN_KEEPALIVE void set_music(int v){if(g_settings)g_settings->setMusic(v);}
EMSCRIPTEN_KEEPALIVE void set_dark(int v) {if(g_settings)g_settings->setDark(v);}
EMSCRIPTEN_KEEPALIVE void set_lefty(int v){if(g_settings)g_settings->setLefty(v);}
EMSCRIPTEN_KEEPALIVE void set_lang(int v) {if(g_settings)g_settings->setLang(v);}
EMSCRIPTEN_KEEPALIVE int  get_sound()     {return g_settings?g_settings->getSound():1;}
EMSCRIPTEN_KEEPALIVE int  get_music()     {return g_settings?g_settings->getMusic():1;}
EMSCRIPTEN_KEEPALIVE int  get_dark()      {return g_settings?g_settings->getDark():1;}
EMSCRIPTEN_KEEPALIVE int  get_lefty()     {return g_settings?g_settings->getLefty():0;}
EMSCRIPTEN_KEEPALIVE int  get_lang()      {return g_settings?g_settings->getLang():0;}

EMSCRIPTEN_KEEPALIVE int get_rec_count()           {return g_scores?g_scores->getCount():0;}
EMSCRIPTEN_KEEPALIVE const char* get_rec_key(int i){return g_scores?g_scores->getKey(i):"";}
EMSCRIPTEN_KEEPALIVE int get_rec_val(int i)        {return g_scores?g_scores->getVal(i):0;}
EMSCRIPTEN_KEEPALIVE int get_record_for_current()  {
    if(!g_scores||!g_game)return 0;
    char key[16]; snprintf(key,16,"%dx%d",g_game->getGridW(),g_game->getGridH());
    return g_scores->getRecord(key);
}

} // extern "C"
