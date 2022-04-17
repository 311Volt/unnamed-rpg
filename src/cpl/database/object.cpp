class ObjectDB
{
    string search_tmp;
    int ObjectIDTracker;
    class Object
    {
    public:
        int w;
        int h;
        int id;
        bool collision;
        string name;
        ALLEGRO_BITMAP* texture;
    };
public:
    vector<Object> objectProto;
    ObjectDB()
    {
        ObjectIDTracker = 0;
    }

    Object* GetObjectByID(int id)
    {
        for(int i=0; i<objectProto.size(); i++)
        {
            if(objectProto[i].id == id)
                return &objectProto[i];
        }
    }

    void ReadObjectProto(string filename)
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
                Object tmp;
                tmp.w = cfg.GetSectionValue(SectionName, "Width");
                tmp.h = cfg.GetSectionValue(SectionName, "Height");
                tmp.collision = cfg.GetSectionValue(SectionName, "Collision");
                tmp.texture = textures.GetBitmapResource(cfg.GetSectionValueS(SectionName, "Texture"));
                tmp.id = ObjectIDTracker;
                tmp.name = SectionName;
                objectProto.push_back(tmp);
                ObjectIDTracker++;
            }
        }
    }
};

ObjectDB objectDB;
