//
// Created by floodd on 26/01/2026.
//

#include "Image.h"
#include <iostream>

using namespace std;
/**
 * This function creates the image object, and then loads in the image from the given filename.
 *
 * @param filename: The name of the file to load
 * /
 */
MyImage::MyImage(string filename): fileName(filename) {
    this->load(filename);
}

/**
 *  This function will reload the original image.
 */
void MyImage::reload() {
    this->load(fileName);
}

/**
 *  This function is responsible for drawing the image scaling to fit on the screen. It also draws a grey
 *  background behind the image to allow for scaling.
 */
void MyImage::draw(sf::RenderTarget &target, sf::RenderStates states)const {

    float x = 0, y = 0;
    int type = this->size.x > this->size.y? LANDSCAPE: PORTRAIT;
    float step=1;
    float startX=this->position.x, startY=this->position.y;
    float dimW = this->targetSize.x;
    float dimH = this->targetSize.y;


    if (this->size.x < this->targetSize.x && this->size.y < this->targetSize.y) {

        dimW = this->size.x;
        startY += (this->targetSize.y - this->size.y)/2.0f;
        dimH = this->size.y;
        startX += (this->targetSize.x- this->size.x)/2.0f;;
    }
    else {
        if (this->size.x / this->targetSize.x > this->size.y / this->targetSize.y) {
            step = this->size.x / this->targetSize.x;
            dimH = this->size.y/step;
            startY += (this->targetSize.y - dimH)/2;;
        }
        else {
            step = this->size.y / (float)this->targetSize.y;
            dimW = this->size.x/step;
            startX +=  (this->targetSize.x - dimW)/2;
        }
    }
    float xStep = 0, yStep=0;
    for (float i = 0; i < dimH; ++i) {
        for (float j = 0; j < dimW; ++j)
        {
            x = (int)xStep;
            y = (int)yStep;
            int pix = (y*this->size.x) + x;
            sf::VertexArray circle(sf::PrimitiveType::Points, 1);
            circle[0].color = sf::Color(this->pixels[pix].r,this->pixels[pix].g,this->pixels[pix].b);
            circle[0].position = {j+startX,i+startY};

            target.draw(circle);
            xStep+=step;
        }
        xStep = 0;
        yStep+=step;
    }
}

/**
* The draw function will scale the image to the maximum defined by this function
*
 * @param target : the target size of the image when displayed.
 */
void MyImage::setTargetSize(sf::Vector2f target) {
    this->targetSize = target;
}

/**
 *
 * @return The size of the original image defined in the PPM file.
 */
sf::Vector2f MyImage::getSize() {
    return this->size;
}

/**
 * This function opens the given ppm file and attempts to read in the data to a continuous vector of pixels defined as
 * RGB structs.
 *
 * @param filename the file to load.
 */
void MyImage::load(string filename) {
    ifstream ifs;

    string dummy;
    ifs.open(filename, std::ios::binary); // need to spec. binary mode for Windows users
    try {
        if (ifs.fail()) { throw("Can't open input file"); }
        std::string header;
        int w, h, b;
        // Reads in the first string to check it starts with "P6" defining a ppm file. If not throws an exception as file
        // can't be read.
        ifs >> header;
        if (strcmp(header.c_str(), "P6") != 0) throw("Can't read input file");
        getline(ifs, dummy);
        /* Checks to see if there is a comment after the first line */
        char c;
        c = ifs.peek();
        if (c == '#') {
            getline(ifs, dummy);
        }
        // Read in the width and height of the image.
        ifs >> w >> h >> b;
        // set the size within the image object
        this->size = {(float)w,(float)h};
        ifs.ignore(256, '\n'); // skip empty lines in necessary until we get to the binary data
        unsigned char pix[3];
        this->pixels.clear();// remove any existing pixel data
        // read each pixel one by one and convert bytes to floats
        for (int i = 0; i < w * h; ++i) {
            ifs.read(reinterpret_cast<char *>(pix), 3);
            RGB pixel;
            pixel.r = pix[0];
            pixel.g = pix[1];
            pixel.b = pix[2];
            this->pixels.push_back(pixel);
        }

        ifs.close();
    }
    catch (const char *err) {
        fprintf(stderr, "%s\n", err);
        ifs.close();
    }
}
void MyImage::save()
{
    string filename = "Output/SavedImage.ppm";
    if (this->size.x == 0 || this->size.y == 0) { fprintf(stderr, "Can't save an empty image\n"); return; }
    std::ofstream ofs;
    try {
        ofs.open(filename, std::ios::binary); // need to spec. binary mode for Windows users
        if (ofs.fail()) throw("Can't open output file");
        ofs << "P6\n" << this->size.x << " " <<  this->size.y << "\n255\n";
        unsigned char r, g, b;
        // loop over each pixel in the image, clamp and convert to byte format
        for (int i = 0; i <  this->size.x *  this->size.y; ++i) {
            r = static_cast<unsigned char>(this->pixels[i].r);
            g = static_cast<unsigned char>(this->pixels[i].g);
            b = static_cast<unsigned char>(this->pixels[i].b);
            ofs << r << g << b;
        }
        ofs.close();
    }
    catch (const char *err) {
        fprintf(stderr, "%s\n", err);
        ofs.close();
    }
}

void MyImage::filterRed()
{
    cout << "Filter Red" << endl;
    for (int i = 0; i < pixels.size(); i++) {
        pixels[i].g = 0;  // Like pixel._green = 0.0
        pixels[i].b = 0;  // Like pixel._blue = 0.0
        // red stays the same
    }
}

void MyImage::filterGreen() {
    cout << "Filter Green" << endl;
    // Set red and blue to 0, keep green
    for (int i = 0; i < pixels.size(); i++) {
        pixels[i].r = 0;
        pixels[i].b = 0;
        // green stays the same
    }
}

void MyImage::filterBlue() {
    cout << "Filter Blue" << endl;
    // Set red and green to 0, keep blue
    for (int i = 0; i < pixels.size(); i++) {
        pixels[i].r = 0;
        pixels[i].g = 0;
        // blue stays the same
    }
}

void MyImage::greyScale() {
    cout << "Filter Greyscale" << endl;
    // Convert to grayscale - similar concept but with 0-255 values
    for (int i = 0; i < pixels.size(); i++) {
        // Convert char to unsigned char for math (0-255 range)
        unsigned char r = static_cast<unsigned char>(pixels[i].r);
        unsigned char g = static_cast<unsigned char>(pixels[i].g);
        unsigned char b = static_cast<unsigned char>(pixels[i].b);

        // Standard grayscale formula
        unsigned char gray = static_cast<unsigned char>(
            0.299f * r + 0.587f * g + 0.114f * b
        );

        // Set all channels to the same value (like making all RGB equal)
        pixels[i].r = static_cast<char>(gray);
        pixels[i].g = static_cast<char>(gray);
        pixels[i].b = static_cast<char>(gray);
    }
}

void MyImage::flipHorizontal() {
    cout << "Flip Horizontal" << endl;
    int width = static_cast<int>(size.x);
    int height = static_cast<int>(size.y);


    vector<RGB> flippedPixels = pixels;  // Create a copy

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int originalIndex = y * width + x;
            int flippedIndex = y * width + (width - 1 - x);
            // Swap pixels horizontally
            flippedPixels[flippedIndex] = pixels[originalIndex];
        }
    }

    pixels = flippedPixels;
}

void MyImage::flipVertical() {
    cout << "Flip Vertical" << endl;
    int width = static_cast<int>(size.x);
    int height = static_cast<int>(size.y);


    vector<RGB> flippedPixels = pixels;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int originalIndex = y * width + x;
            int flippedIndex = (height - 1 - y) * width + x;
            // Swap pixels vertically
            flippedPixels[flippedIndex] = pixels[originalIndex];
        }
    }

    pixels = flippedPixels;
}

// Ali did this advanced Feature which involves Rotation
void MyImage::advancedFeature1() {
    cout << "Advanced Feature - Rotate 90 Degrees Clockwise" << endl;

    int oldWidth = static_cast<int>(size.x);
    int oldHeight = static_cast<int>(size.y);

    // Create new vector for rotated image
    vector<RGB> rotatedPixels;
    rotatedPixels.resize(oldWidth * oldHeight);

    // Perform rotation
    for (int y = 0; y < oldHeight; y++) {
        for (int x = 0; x < oldWidth; x++) {
            int oldIndex = y * oldWidth + x;
            // For 90° clockwise: newX = oldY, newY = oldWidth - 1 - oldX
            int newX = oldHeight - 1 - y;
            int newY = x;
            int newIndex = newY * oldHeight + newX;
            rotatedPixels[newIndex] = pixels[oldIndex];
        }
    }

    // Swap width and height
    size.x = static_cast<float>(oldHeight);
    size.y = static_cast<float>(oldWidth);

    pixels = rotatedPixels;
}

//Aman did this advanced feature which is cropping
void MyImage::advancedFeature2() {
    cout << "Advanced Feature - Crop to Center" << endl;

    int width = static_cast<int>(size.x);
    int height = static_cast<int>(size.y);

    // Crop to center 50% of the image
    int newWidth = width / 2;
    int newHeight = height / 2;
    int startX = width / 4;
    int startY = height / 4;

    vector<RGB> croppedPixels;
    croppedPixels.resize(newWidth * newHeight);

    // Copy cropped area
    for (int y = 0; y < newHeight; y++) {
        for (int x = 0; x < newWidth; x++) {
            int oldIndex = (startY + y) * width + (startX + x);
            int newIndex = y * newWidth + x;
            croppedPixels[newIndex] = pixels[oldIndex];
        }
    }

    // Update size
    size.x = static_cast<float>(newWidth);
    size.y = static_cast<float>(newHeight);
    pixels = croppedPixels;
}

// Additional Feature Applying Postereies, we did it together...
void MyImage::advancedFeature3() {
    cout << "Applying Posterize Effect (4 levels)" << endl;

    int levels = 4;  // Number of levels per channel
    int step = 256 / levels;  // Size of each level

    for (int i = 0; i < pixels.size(); i++) {
        // Process each channel
        unsigned char r = static_cast<unsigned char>(pixels[i].r);
        unsigned char g = static_cast<unsigned char>(pixels[i].g);
        unsigned char b = static_cast<unsigned char>(pixels[i].b);

        // Quantize each channel to nearest level
        r = static_cast<unsigned char>((r / step) * step + step/2);
        g = static_cast<unsigned char>((g / step) * step + step/2);
        b = static_cast<unsigned char>((b / step) * step + step/2);

        // Ensure values stay in range
        if (r > 255) r = 255;
        if (g > 255) g = 255;
        if (b > 255) b = 255;

        pixels[i].r = static_cast<char>(r);
        pixels[i].g = static_cast<char>(g);
        pixels[i].b = static_cast<char>(b);
    }
}


