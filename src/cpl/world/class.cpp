

class world
{
    int w;
    int h;
    class Building
    {
    public:
        int rot;
        int x, y;
        int id;
    };
    class Object
    {
    public:
        int rot;
        int x, y;
        int id;
    };

    class Block
    {
    public:
        block()
        {
            id = 0;
            rotation = 0;
        }
        unsigned char id;
        unsigned char rotation;
    };

    class light_environment
    {
    public:
        ALLEGRO_COLOR color;
        double angle;
        double length;
        int softness;
    };

    int building_id_tracker;

public:
    vector<Block> blockdata;
    vector<Building> building;
    vector<Object> object;
    bool CheckValidPos(int x, int y)
    {
        if(x < 0 || x >= w || y < 0 || y >= h)
            return false;
        return true;
    }
    void SetBlock(int x, int y, int rotation, int type)
    {
        if(CheckValidPos(x,y))
        {
            blockdata[y*w+x].id = type;
            blockdata[y*w+x].rotation = rotation;
            if(blockDB.GetBlockByID(type)->random_rotation)
                blockdata[y*w+x].rotation = rand()%4;
        }
    }
    void FillBlock(int type)
    {
        blockdata.clear();
        blockdata.resize(w*h);
        for(int i=0; i<w; i++)
        {
            for(int j=0; j<h; j++)
            {
                SetBlock(i,j,0,type);
            }
        }
    }
    void Resize(int newW, int newH)
    {
        vector<Block> blockdata_new;
        Block zero;
        zero.id = 0;
        zero.rotation = 0;
        for(int i=0; i<newH; i++)
        {
            for(int j=0; j<newW; j++)
            {
                if(i<h && j<w)
                {
                    blockdata_new.push_back(blockdata[i*h+j]);
                }
                else blockdata_new.push_back(zero);
                zero.rotation = rand()%3;
            }
        }
        blockdata.clear();
        blockdata = blockdata_new;
        blockdata_new.clear();
        w = newW;
        h = newH;
    }
    void SetSize(int newW, int newH)
    {
        w = newW;
        h = newH;
        FillBlock(0);
    }
    int GetBlock(int x, int y)
    {
        return blockdata[y*w+x].id;
    }
    int GetRotation(int x, int y)
    {
        return blockdata[y*w+x].rotation;
    }
    void RotateBlockClockwise(int x, int y)
    {
        if(CheckValidPos(x,y))
        {
            blockdata[y*w+x].rotation++;
            if(blockdata[y*w+x].rotation == 4)
                blockdata[y*w+x].rotation = 0;

        }
    }
    void RotateBlockCounterClockwise(int x, int y)
    {
        if(CheckValidPos(x,y))
        {
            blockdata[y*w+x].rotation--;
            if(blockdata[y*w+x].rotation == -1)
                blockdata[y*w+x].rotation = 3;

        }
    }

    int GetBuildingFromPosition(int x, int y)
    {
        for(int i=0; i<building.size(); i++)
        {
            int _w = buildingDB.GetBuildingByID(building[i].id)->w-1;
            int _h = buildingDB.GetBuildingByID(building[i].id)->h-1;
            if(x >= building[i].x &&
               y >= building[i].y &&
               x <= building[i].x + _w &&
               y <= building[i].y + _h)
                return i+1;
        }
        return 0;
    }

    void DeleteBuildingAtPosition(int x, int y)
    {
        if(GetBuildingFromPosition(x,y))
            building.erase(building.begin()+GetBuildingFromPosition(x,y)-1);
    }

    bool SafeToPlaceBuilding(int x, int y, int id)
    {
        int _w = buildingDB.GetBuildingByID(id)->w-1;
        int _h = buildingDB.GetBuildingByID(id)->h-1;

        if(GetBuildingFromPosition(x,y)) return false;
        if(GetBuildingFromPosition(x+_w, y)) return false;
        if(GetBuildingFromPosition(x,y+_h)) return false;
        if(GetBuildingFromPosition(x+_w, y+_h)) return false;

        return true;
    }
    void InsertBuilding(int x, int y, int rot, int id)
    {
        Building tmp;
        tmp.x = x;
        tmp.y = y;
        tmp.id = id;
        tmp.rot = rot;
        building.push_back(tmp);
    }

    bool SafeToPlaceObject(int x, int y, int id)
    {
        int _w = objectDB.GetObjectByID(id)->w-1;
        int _h = objectDB.GetObjectByID(id)->h-1;

        if(GetObjectFromPosition(x,y)) return false;
        if(GetObjectFromPosition(x+_w, y)) return false;
        if(GetObjectFromPosition(x,y+_h)) return false;
        if(GetObjectFromPosition(x+_w, y+_h)) return false;

        return true;
    }

    int GetObjectFromPosition(int x, int y)
    {
        for(int i=0; i<object.size(); i++)
        {
            int _w = objectDB.GetObjectByID(object[i].id)->w-1;
            int _h = objectDB.GetObjectByID(object[i].id)->h-1;
            if(x >= object[i].x &&
               y >= object[i].y &&
               x <= object[i].x + _w &&
               y <= object[i].y + _h)
                return i+1;
        }
        return 0;
    }

    void InsertObject(int x, int y, int rot, int id)
    {
        Object tmp;
        tmp.x = x;
        tmp.y = y;
        tmp.id = id;
        tmp.rot = rot;
        object.push_back(tmp);
    }

    void DeleteObjectAtPosition(int x, int y)
    {
        if(GetObjectFromPosition(x,y))
            object.erase(object.begin()+GetObjectFromPosition(x,y)-1);
    }

    int GetWidth() {return w;}
    int GetHeight(){return h;}


    void SaveToFile(string filename)
    {
        /* system zapisu:
        HEADER
        4 B - szerokoœæ
        4 B - wysokoœæ
        4 B - iloœæ budynków
        4 B - iloœæ obiektów

        BLOCKDATA (n = w*h)
        1 B - ID
        1 B - obrót

        BUILDING DATA (n = [address]04h)
        1 B - ID
        1 B - obrót
        4 B - X
        4 B - Y

        OBJECT DATA (n=[address]06h)
        1 B - ID
        1 B - obrót
        4 B - X
        4 B - Y
        */

        int b_size = building.size();
        int o_size = object.size();

        ofstream save(filename.c_str(), ios::binary);

        //HEADER
        save.write((char*)&w, 4);
        save.write((char*)&h, 4);
        save.write((char*)&b_size, 4);
        save.write((char*)&o_size, 4);

        //BLOCKDATA
        for(int i=0; i<blockdata.size(); i++)
        {
            save.write((char*)&blockdata[i].id, 1);
            save.write((char*)&blockdata[i].rotation, 1);
        }

        //BUILDING DATA
        for(int i=0; i<b_size; i++)
        {
            save.write((char*)&building[i].id, 4);
            save.write((char*)&building[i].rot, 4);
            save.write((char*)&building[i].x, 4);
            save.write((char*)&building[i].y, 4);
        }

        //OBJECT DATA
        for(int i=0; i<o_size; i++)
        {
            save.write((char*)&object[i].id, 4);
            save.write((char*)&object[i].rot, 4);
            save.write((char*)&object[i].x, 4);
            save.write((char*)&object[i].y, 4);

            cout<<"OBJECT: "<<object[i].id<<" / "<<object[i].rot<<" / "<<object[i].x<<","<<object[i].y<<endl;
        }
        save.close();
        cout<<"Wrote "<<w*h<<" blocks, "<<b_size<<"buildings and "<<o_size<<"objects"<<endl;
    }


    world()
    {
        building_id_tracker = 69;
        w = 1;
        h = 1;
    }
    ~world()
    {
        blockdata.clear();
        building.clear();
        object.clear();
    }
};

world* CreateEmptyWorld(int w, int h, int fill)
{
    world* tmp = new world;
    tmp->SetSize(w,h);
    tmp->FillBlock(fill);
    return tmp;
}

world* CreateWorldFromFile(string filename)
{
    int w;
    int h;
    int buildings;
    int objects;

    unsigned char id8;
    unsigned char rot8;

    int id;
    int rot;
    int x;
    int y;

    ifstream save(filename.c_str(), ios::binary);
    world* tmp = new world;

    save.read((char*)&w, 4);
    save.read((char*)&h, 4);
    save.read((char*)&buildings, 4);
    save.read((char*)&objects, 4);
    cout<<"MAP SIZE: "<<w<<","<<h<<endl;
    tmp->SetSize(w,h);
    tmp->blockdata.resize(w*h);

    for(int i=0; i<w*h; i++)
    {
        save.read((char*)&id8, 1);
        save.read((char*)&rot8, 1);

        tmp->blockdata[i].rotation = rot8;
        tmp->blockdata[i].id = id8;

        if(i<=20 || i>=w*h-20) cout<<"BLOCK #"<<i<<": "<<(int)tmp->blockdata[i].id<<"/"<<(int)tmp->blockdata[i].rotation<<endl;
    }

    for(int i=0; i<buildings; i++)
    {
        save.read((char*)&id, 4);
        save.read((char*)&rot, 4);
        save.read((char*)&x, 4);
        save.read((char*)&y, 4);

        if(save.eof())
        {
            buildings = i;
            break;
        }

        tmp->InsertBuilding(x, y, rot, id);

        cout<<"BUILDING: "<<id<<" / "<<rot<<" / "<<x<<","<<y<<endl;
    }

    for(int i=0; i<objects; i++)
    {
        save.read((char*)&id, 4);
        save.read((char*)&rot, 4);
        save.read((char*)&x, 4);
        save.read((char*)&y, 4);

        if(save.eof())
        {
            objects = i+1;
            break;
        }

        tmp->InsertObject(x, y, rot, id);
        cout<<"OBJECT: "<<id<<" / "<<rot<<" / "<<x<<","<<y<<endl;
    }
    cout<<"Read "<<w*h<<" blocks, "<<buildings<<"buildings and "<<objects<<"objects"<<endl;
    return tmp;
}
