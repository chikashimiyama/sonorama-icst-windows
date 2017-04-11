class SphereModel : public Model{

public:
    SphereModel(SInt64 id, std::vector<ofVec3f> vertices, std::unordered_map<std::string, std::string> tags):
    Model(id,vertices,tags),
    radius(100.0){}
    
    virtual void draw() const override{
        ofSetColor(255, 125, 125, 20);
        ofDrawSphere(position, radius);
    }
    
    virtual void label(const Camera &camera) const override{
        ofSetColor(255, 125, 125, 255);
        Model::label(camera);
    }
    
    void setRadius(float r){
        radius = r;
    }
    
private:
    float radius;
};
