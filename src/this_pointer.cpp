class Test final
{
public:
    void increase() { ++this; }
    void decrease() { --this; }
    Test* next() const { return this + 1; }
    Test* prev() const { return this - 1; }
};
