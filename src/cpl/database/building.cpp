class BuildingDB
{
    string search_tmp;
    int BuildingIDTracker;
    class Building
    {
    public:
        int height;
        int w;
        int h;
        int id;
        string name;
        ALLEGRO_BITMAP* texture[3];
    };
public:
    vector<Building> buildingProto;
    BuildingDB()
    {
        BuildingIDTracker = 0;
    }

    Building* GetBuildingByID(int id)
    {
        for(int i=0; i<buildingProto.size(); i++)
        {
            if(buildingProto[i].id == id)
                return &buildingProto[i];
        }
    }

    void ReadBuildingProto(string filename)
    {
        ifstream bproto(filename.c_str());
        if(!bproto.good())
            return;
        ConfigFile cfg;
        cfg.LoadFile(filename.c_str());
        while(!bproto.eof())
        {
            getline(bproto, search_tmp);
            if(search_tmp[0] == '[' && search_tmp[search_tmp.size()-1] == ']')
            {
                string SectionName; for(int i=1; i<search_tmp.size()-1; i++) SectionName += search_tmp[i];
                Building tmp;
                tmp.height = cfg.GetSectionValue(SectionName, "3D_Height");
                tmp.w = cfg.GetSectionValue(SectionName, "Width");
                tmp.h = cfg.GetSectionValue(SectionName, "Height");
                tmp.texture[0] = textures.GetBitmapResource(cfg.GetSectionValueS(SectionName, "TextureFront"));
                tmp.texture[1] = textures.GetBitmapResource(cfg.GetSectionValueS(SectionName, "TextureSide"));
                tmp.texture[2] = textures.GetBitmapResource(cfg.GetSectionValueS(SectionName, "TextureRoof"));
                tmp.id = BuildingIDTracker;
                tmp.name = SectionName;
                buildingProto.push_back(tmp);
                BuildingIDTracker++;
            }
        }
    }
};

BuildingDB buildingDB;
