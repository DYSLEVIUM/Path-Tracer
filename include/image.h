template <typename T>
class Image {
   private:
    T width;
    T height;

   public:
    Image() : width(256), height(256){};

    Image(const T wid, const T hei) {
        this->width = wid;
        this->height = hei;
    }

    T getWidth() const {
        return this->width;
    }

    const T getHeight() const {
        return this->height;
    }

    ~Image(){};
};