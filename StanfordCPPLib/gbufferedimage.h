/**
 * @file gbufferedimage.h
 *
 * @brief
 * This file exports the GBufferedImage class for per-pixel graphics.
 */

/*************************************************************************/
/* Stanford Portable Library                                             */
/*                                                                       */
/* This program is free software: you can redistribute it and/or modify  */
/* it under the terms of the GNU General Public License as published by  */
/* the Free Software Foundation, either version 3 of the License, or     */
/* (at your option) any later version.                                   */
/*                                                                       */
/* This program is distributed in the hope that it will be useful,       */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of        */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         */
/* GNU General Public License for more details.                          */
/*                                                                       */
/* You should have received a copy of the GNU General Public License     */
/* along with this program.  If not, see <http://www.gnu.org/licenses/>. */
/*************************************************************************/

#ifndef _gbufferedimage_h
#define _gbufferedimage_h

#include "grid.h"
#include "ginteractors.h"
#include "gobjects.h"
#include "gtypes.h"

// default color used to highlight pixels that do not match between two images
#define GBUFFEREDIMAGE_DEFAULT_DIFF_PIXEL_COLOR 0xdd00dd

/**
 * This interactor subclass implements a 2D region of colored pixels that can be read/set
 * individually, not unlike the <code>BufferedImage</code> class in Java.
 * The color of each pixel in a \c %GBufferedImage is represented
 * as a 32-bit integer in the range from `0x000000` to `0xffffff`. Such a color
 * value has the form <code>0x</code><i>rrggbb</i> and represents the
 * color having red, green, and blue components given by the hex values
 * \em rr, \em gg, and \em bb, respectively. Each component can take on any
 * of the 256 values from 0 through 255 (00 through ff, in hexadecimal).
 *
 * For example, the integer <code>0xff00cc</code> is a purple color, while
 * <code>0x770000</code> is a dark red.
 * A \c %GBufferedImage will also let you represent colors as hex strings
 * such as <code>"#ff00cc"</code> instead of as integers.
 * String colors are always converted to integer colors internally with the help
 * of \ref convertColorToRGB.
 * Therefore some common color names like \c "black" can be passed successfully
 * in lieu of the equivalent hex string; see that function for more details.
 *
 * Though the \em x, \em y, \em width, and \em height parameters to the constructor and several
 * methods are received as type <code>double</code> for maximum compatibility
 * with other classes in this library, they should be thought of as
 * integer coordinates.  Any real numbers passed will be internally truncated
 * into integers by discarding any fractional component (by typecasting from
 * <code>double</code> to <code>int</code>).
 *
 * Note that per-pixel graphics operations using the Stanford C++ library are
 * relatively slow.  A call to the \ref fill method is relatively
 * efficient, and a call to \ref getRGB is also efficient since pixels'
 * colors are cached locally.  But calling \ref setRGB repeatedly over
 * a large range of pixels is likely to yield poor performance.
 * This is due to the fact that the graphics are implemented using a background
 * Java process to which all graphical commands are forwarded.
 * The \c %GBufferedImage class is not performant enough to be used
 * for rapidly advancing animations or extremely complex graphical effects.
 * It also does not provide any drawing primitives other than filling
 * individual pixels and rectangular regions.
 * If you want to draw shapes and lines, use other classes from this library
 * such as GRect, GLine, and so on.
 */
class GBufferedImage : public GInteractor {
public:
    /** \_overload */
    GBufferedImage();
    //GBufferedImage(double width, double height);
    /** \_overload */
    GBufferedImage(double width, double height,
                   int rgbBackground = 0x000000);
    /** \_overload */
    GBufferedImage(double x, double y, double width, double height,
                   int rgbBackground = 0x000000);
    /**
     * Constructs an image with the specified location, size, and optional
     * background color.
     * If no size is passed, the default size of 0-by-0 pixels is used.
     * If no location is passed, the default of `(0, 0)` is used.
     * If no background color is passed, the default of black (`0x000000`) is used.
     * Throws an error if \em width or \em height is negative.
     * Throws an error if the given \em rgb value is invalid or out of range.
     *
     * Sample usages:
     *
     *     GBufferedImage *im = new GBufferedImage();
     *     GBufferedImage *im = new GBufferedImage(width, height);
     *     GBufferedImage *im = new GBufferedImage(width, height, rgbBackground);
     *     GBufferedImage *im = new GBufferedImage(x, y, width, height, rgbBackground);
     */
    GBufferedImage(double x, double y, double width, double height,
                   std::string rgbBackground);
    
    /* Prototypes for the virtual methods */
    virtual GRectangle getBounds() const;
    virtual std::string getType() const;
    virtual std::string toString() const;

    /* unique GBufferedImage behavior */

    /**
     * Sets all pixels in this image to be the original background color passed
     * to the constructor.
     *
     * Sample usage:
     *
     *     im->clear();
     */
    void clear();
    

    /**
     * Returns the total number of pixels that are not the same color
     * between this image and the given image.
     * If the images are not the same size, any pixels in the range of one image
     * but out of the bounds of the other are considered to be differing.
     *
     * Sample usage:
     *
     *     int n = im->countDiffPixels(other);
     */
    int countDiffPixels(GBufferedImage& image) const;
    

    /**
     * Generates a new image with the same size and background color as this image, but
     * with any pixels that don't match those in given image colored in the given
     * color \em diffPixelColor (default purple) to highlight differences between the two.
     *
     * Sample usages:
     *
     *     GBufferedImage *diffIm = im->diff(im2);
     *     GBufferedImage *diffIm = im->diff(im2, diffPixelColor);
     */
    GBufferedImage* diff(GBufferedImage& image, int diffPixelColor = GBUFFEREDIMAGE_DEFAULT_DIFF_PIXEL_COLOR) const;
    

    /** \_overload */
    void fill(int rgb);
    /**
     * Sets the color of every pixel in this image to the given color value \em rgb,
     * which may may be specified as an integer value or as a string like
     * `"#ff00cc"` or `"blue"`.
     * Throws an error if \em rgb value is not a valid color.
     *
     * Sample usage:
     *
     *     im->fill(rgb);
     */
    void fill(std::string rgb);


    /** \_overload */
    void fillRegion(double x, double y, double width, double height, int rgb);
    /**
     * Sets the color of every pixel in the given rectangular region of this image
     * to the given color value \em rgb.
     * Specifically, the pixels in the rectangular range (\em x, \em y) through
     * (\em x + \em width - 1, \em y + \em height - 1) become filled with the given color.
     * Throws an error if the given x/y/width/height range goes outside the bounds
     * of the image.
     * Throws an error if \em rgb is not a valid color.
     *
     * Sample usage:
     *
     *     im->fillRegion(x, y, width, height, rgb)
     */
    void fillRegion(double x, double y, double width, double height,
                    std::string rgb);


    /**
     * Returns the height of this image in pixels.
     *
     * Sample usage:
     *
     *     double h = im->getHeight();
     */
    double getHeight() const;


    /**
     * Returns the color of the pixel at the given (\em x, \em y) coordinates of this image
     * as an integer such as \c 0xff00cc.
     * Throws an error if the given coordinates are out of bounds.
     *
     * Sample usage:
     *
     *     int rgb = im->getRGB(x, y);
     */
    int getRGB(double x, double y) const;


    /**
     * Returns the color of the pixel at the given (\em x, \em y) coordinates of this image
     * as a string such as `"#ff00cc"`.
     * Throws an error if the given coordinates are out of bounds.
     *
     * Sample usage:
     *
     *     string rgb = im->getRGB(x, y);
     */
    std::string getRGBString(double x, double y) const;


    /**
     * Returns the width of this image in pixels.
     *
     * Sample usage:
     *
     *     double w = im->getWidth();
     */
    double getWidth() const;
    

    /**
     * Returns \c true if the given (\em x, \em y) position is within the range of pixels
     * occupied by this image.
     *
     * Sample usage:
     *
     *     if (im->inBounds(x, y)) ...
     */
    bool inBounds(double x, double y) const;
    

    /**
     * Reads this image's contents from the specified image file.
     * The file must have one of the following formats:
     * <ul>
     *  <li>Bitmap (BMP) format
     *  <li>Graphics Interchange Format (GIF)
     *  <li>Joint Photographic Experts Group (JPEG) format
     *  <li>Portable Network Graphics (PNG) format
     * </ul>
     * If the image file is successfully loaded, this image's dimensions are
     * set to match the dimensions of the loaded image.
     *
     * This method throws an error if the file could not be found, could not be
     * opened for reading, or is not a valid image file in one of the supported formats.
     *
     * Sample usage:
     *
     *     im->load(filename);
     */
    void load(const std::string& filename);
    

    /**
     * Changes this image's bounds to be the given size.
     * This does not scale the image but rather just changes the range
     * of (\em x, \em y) locations that are considered to be within the
     * bounds of the image.
     * If the \em retain parameter is not passed or is set to \c true,
     * any existing pixel values will be kept during the resize.
     * If \em retain is false, the contents will be erased and set to the background
     * color of this image.
     * Throws an error if \em width or \em height ranges is negative.
     *
     * Sample usages:
     *
     *     im->resize(width, height);
     *     im->resize(width, height, false);
     */
    void resize(double width, double height, bool retain = true);
    

    /**
     * Returns a pointer to a new GBufferedImage that is a scaled version
     * of this one, scaled to the given width and height.
     *
     * Sample usages:
     *
     *     GBufferedImage *scaledImage = im->scale(width, height);
     */
    GBufferedImage *scale(int width, int height) const;


    /**
     * Saves this image's contents to the given image file. The format
     * of the image file is determined by the extension of \em filename.
     * The supported formats and corresponding filename extensions are
     * as follows:
     *
     * <table>
     * <tr>
     *      <td>Bitmap (BMP)</td>
     *      <td><code>.bmp</code></td>
     *  </tr>
     *  <tr>
     *      <td>Graphics Interchange Format (GIF)</td>
     *      <td><code>.gif</code></td>
     *  </tr>
     *  <tr>
     *      <td>Joint Photographic Experts Group (JPEG)</td>
     *      <td><code>.jpg</code>, <code>.jpeg</code></td>
     *  </tr>
     *  <tr>
     *      <td>QuickDraw Native Format (PICT)</td>
     *      <td><code>.pic</code>, <code>.pict</code></td>
     *  </tr>
     *  <tr>
     *      <td>Portable Network Graphics (PNG)</td>
     *      <td><code>.png</code></td>
     *  </tr>
     *  <tr>
     *      <td>Tagged Image File Format (TIFF)</td>
     *      <td><code>.tif</code>, <code>.tiff</code></td>
     *  </tr>
     * </table>
     *
     * This method throws an error if the given file is not writeable or the
     * requested image file format is not supported.
     *
     * Sample usage:
     *
     *     im->save(filename);
     */
    void save(const std::string& filename) const;


    /** \_overload */
    void setRGB(double x, double y, int rgb);
    /**
     * Sets the color of the pixel at the given <i>xy</i>-coordinates of this
     * image to the given value.
     * Implementation/performance note: Each call to this method produces a
     * call to the Java graphical back-end.  Calling this method many times
     * in a tight loop can lead to poor performance.  If you need to fill a
     * large rectangular region, consider calling \ref fill or \ref fillRegion
     * instead.
     * Throws an error if the given (\em x, \em y) values are out of bounds.
     * Throws an error if \em rgb is not a valid color.
     *
     * Sample usage:
     *
     *     im->setRGB(x, y, rgb);
     */
    void setRGB(double x, double y, std::string rgb);

private:
    double m_width;          // really, these are treated as integers
    double m_height;
    int m_backgroundColor;
    Grid<int> m_pixels;      // row-major; [y][x]

    /*
     * Throws an error if the given rgb value is not a valid color.
     */
    void checkColor(std::string member, int rgb) const;

    /*
     * Throws an error if the given x/y values are out of bounds.
     */
    void checkIndex(std::string member, double x, double y) const;

    /*
     * Throws an error if the given width/height values are out of bounds.
     */
    void checkSize(std::string member, double width, double height) const;

    /*
     * Initializes private member variables; called by all constructors.
     */
    void init(double x, double y, double width, double height, int rgb);
};

#endif
