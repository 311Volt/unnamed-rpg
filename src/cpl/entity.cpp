

class entity
{
    string id;
    string world_id;
    long double x;
    long double y;
    long double speedx;
    long double speedy;
    long double forcex;
    long double forcey;
    long double speedlimit;
    
    ALLEGRO_BITMAP* texture;
    
public:
    entity()
    {
        texture = NULL; speedlimit = 0;
        x = 0; y = 0; speedx = 0; speedy = 0; forcex = 0; forcey = 0;
        speedlimit = 0;
        world_id = "";
    }
    ~entity()
    {
        //if(texture != NULL)
            //al_destroy_bitmap(texture);
    }
    //settery
    void SetID(string input) {id = input;}
    void SetX(double input) {x = input;}
    void SetY(double input) {y = input;}
    void MoveX(double input) {x += input;}
    void MoveY(double input) {y += input;}
    void SetSpeedX(double input) {speedx = input;}
    void SetSpeedY(double input) {speedy = input;}
    void AddSpeedX(double input) {speedx += input;}
    void AddSpeedY(double input) {speedy += input;}
    void SetForceX(double input) {forcex = input;}
    void SetForceY(double input) {forcey = input;}
    void AddForceX(double input) {forcex += input;}
    void AddForceY(double input) {forcey += input;}
    void SetSpeedLimit(double input) {speedlimit = input;}
    void MoveToWorldID(string input) {world_id = input;}
    void SetTexture(ALLEGRO_BITMAP* input) {texture = input;}
    
    //gettery
    double GetX() {return x;}
    double GetY() {return y;}
    double GetSpeedX() {return speedx;}
    double GetSpeedY() {return speedy;}
    double GetForceX() {return forcex;}
    double GetForceY() {return forcey;}
    double GetSpeedLimit() {return speedlimit;};
    string GetID() {return id;}
    string GetWorldID() {return world_id;};
    ALLEGRO_BITMAP* GetTexture() {return texture;}
    
    
    //inne metody
    void Update(double frametime, double grip)
    {
        //cout<<"Force X: "<<forcex;
        speedx += forcex*grip*frametime*40;
        speedy += forcey*grip*frametime*40;
        //cout<<", Pre-Spd X: "<<speedx;
        if(speedlimit != 0)
        {
            if(speedx > speedlimit) speedx = speedlimit;
            if(speedy > speedlimit) speedy = speedlimit;
            if(speedx < 0) if(speedx < -speedlimit) speedx = -speedlimit;
            if(speedy < 0) if(speedy < -speedlimit) speedy = -speedlimit;
        }
        //cout<<", Post-Spd X: "<<speedx<<endl;
        forcex = -speedx/120;
        forcey = -speedy/120;
        x += speedx*frametime;
        y += speedy*frametime;
    }
    
    
};
