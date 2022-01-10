class Mode 
{
public:
    virtual ~Mode() {}
    virtual void perform() const = 0;
}