package edu.stanford.cs.java.spl;

import edu.stanford.cs.java.graphics.GImageTools;
import java.awt.*;
import java.awt.image.*;
import java.io.*;
import javax.imageio.*;
import javax.swing.*;

/**
 * 
 * @author Marty Stepp
 * @version 2014/10/22
 * 
 * Jeff Lutgen added scale method.
 */
public class GBufferedImage extends GInteractor {
	
	private BufferedImage bufferedImage;
	private int imageWidth;
	private int imageHeight;
	private int backgroundColor;
	private JLabel label;
	
	public GBufferedImage(int width, int height) {
		this(width, height, 0);   // black
	}
	
	public GBufferedImage(int width, int height, int backgroundColor) {
		super(new JLabel("HOORAY"));
		this.imageWidth = width;
		this.imageHeight = height;
		this.backgroundColor = backgroundColor;
		bufferedImage = new BufferedImage(width, height, BufferedImage.TYPE_INT_RGB);
		label = (JLabel) this.getInteractor();
		if (backgroundColor != 0) {
			fill(backgroundColor);
		}
		repaintImage();
	}
	
	public void clear() {
		fill(backgroundColor);
	}
	
	public void fill(int rgb) {
		for (int y = 0; y < imageHeight; y++) {
			for (int x = 0; x < imageWidth; x++) {
				bufferedImage.setRGB(x, y, rgb);
			}
		}
		repaintImage();
	}
	
	public void fillRegion(int x, int y, int width, int height, int rgb) {
		for (int yy = y; yy < y + height; yy++) {
			for (int xx = x; xx < x + width; xx++) {
				bufferedImage.setRGB(xx, yy, rgb);
			}
		}
		repaintImage();
	}
	
	public BufferedImage getBufferedImage() {
		return bufferedImage;
	}
	
	public int getImageHeight() {
		return imageHeight;
	}
	
	public int getImageWidth() {
		return imageWidth;
	}
	
	public String load(String filename) {
		try {
			bufferedImage = ImageIO.read(new File(filename));
			if (bufferedImage == null)
				throw (new RuntimeException("Could not load image file: unsupported file format"));
			imageWidth = bufferedImage.getWidth();
			imageHeight = bufferedImage.getHeight();
			repaintImage();
		    return toStringBase64(); // this is a LONG string
		} catch (Exception e) {
			throw (new RuntimeException(e.getMessage()));
		}
	}
	
	public void resize(int w, int h, boolean retain) {
		BufferedImage oldImage = bufferedImage;
		bufferedImage = new BufferedImage(w, h, BufferedImage.TYPE_INT_RGB);
		imageWidth = w;
		imageHeight = h;
		fill(backgroundColor);
		if (retain) {
			Graphics g = bufferedImage.getGraphics();
			g.drawImage(oldImage, 0, 0, label);
		}
		repaintImage();
	}
	
	public void save(String filename) {
		GImageTools.saveImage(bufferedImage, filename);
	}
	
	public void setRGB(int x, int y, int rgb) {
		bufferedImage.setRGB(x, y, rgb);
		repaintImage();
	}
	
	public String toStringBase64() {
		StringBuilder sb = new StringBuilder(1024 * 256);
		sb.append((int) imageWidth);
		sb.append('\n');
		sb.append((int) imageHeight);
		sb.append('\n');
		for (int y = 0; y < imageHeight; y++) {
			for (int x = 0; x < imageWidth; x++) {
				// trim off top two hex digits (alpha) because C++ lib expects 24-bit ints
				int px = bufferedImage.getRGB(x, y);
				sb.append(String.format("#%06x\n", px & 0x00ffffff));
			}
		}
		String base64 = Base64.encodeBytes(sb.toString().getBytes());
		return base64;
	}
	
	// JL modified this method so that calls to thread-unsafe Swing methods
	// occur on the Event Dispatch Thread.
	private void repaintImage() {
		SwingUtilities.invokeLater(new Runnable() {
	        public void run() {
	        	label.setIcon(new ImageIcon(bufferedImage));
	    		Dimension size = new Dimension(imageWidth, imageHeight);
	    		label.setPreferredSize(size);
	    		label.setSize(size);	        }
	    });		
	}
	
	/**
     * Creates a scaled instance of this {@code GBufferedImage}'s 
     * image and stores it as targetImage's image. Returns Base64-encoded 
     * string representation of targetImage.
     *
     * Adapted by Jeff Lutgen from 
     * https://today.java.net/pub/a/today/2007/04/03/perils-of-image-getscaledinstance.html
     *
     * @param targetWidth the desired width of the scaled instance,
     *    in pixels
     * @param targetWidth the desired width of the scaled instance,
     *    in pixels
     * @param targetHeight the desired height of the scaled instance,
     *    in pixels
     * 
     * hint: one of the rendering hints that corresponds to
     *    {@code RenderingHints.KEY_INTERPOLATION} (e.g.
     *    {@code RenderingHints.VALUE_INTERPOLATION_NEAREST_NEIGHBOR},
     *    {@code RenderingHints.VALUE_INTERPOLATION_BILINEAR},
     *    {@code RenderingHints.VALUE_INTERPOLATION_BICUBIC})
     *    
     * higherQuality: if true, this method will use a multi-step
     *    scaling technique that provides higher quality than the usual
     *    one-step technique (only useful in downscaling cases, where
     *    {@code targetWidth} or {@code targetHeight} is
     *    smaller than the original dimensions, and generally only when
     *    the {@code BILINEAR} hint is specified)
     *    
     * @return a scaled version of the original {@code BufferedImage}
     */
    public String scale(GBufferedImage targetImage,
    									   int targetWidth,
                                           int targetHeight)
    {
    	boolean higherQuality;
    	Object hint;
    	if (targetWidth < imageWidth || targetHeight < imageHeight) {
    		higherQuality = true;
    		hint = RenderingHints.VALUE_INTERPOLATION_BILINEAR;
    	} else {
    		higherQuality = false;
    		hint = RenderingHints.VALUE_INTERPOLATION_BICUBIC;
    	}
        int type = BufferedImage.TYPE_INT_RGB;
        BufferedImage ret = (BufferedImage) bufferedImage;
        int w, h;
        if (higherQuality) {
            // Use multi-step technique: start with original size, then
            // scale down in multiple passes with drawImage()
            // until the target size is reached
            w = bufferedImage.getWidth();
            h = bufferedImage.getHeight();
        } else {
            // Use one-step technique: scale directly from original
            // size to target size with a single drawImage() call
            w = targetWidth;
            h = targetHeight;
        }
        
        do {
            if (higherQuality && w > targetWidth) {
                w /= 2;
                if (w < targetWidth) {
                    w = targetWidth;
                }
            }

            if (higherQuality && h > targetHeight) {
                h /= 2;
                if (h < targetHeight) {
                    h = targetHeight;
                }
            }

            BufferedImage tmp = new BufferedImage(w, h, type);
            Graphics2D g2 = tmp.createGraphics();
            g2.setRenderingHint(RenderingHints.KEY_INTERPOLATION, hint);
            g2.drawImage(ret, 0, 0, w, h, null);
            g2.dispose();

            ret = tmp;
        } while (w != targetWidth || h != targetHeight);
        
        targetImage.bufferedImage = ret;
        targetImage.imageWidth = ret.getWidth();
        targetImage.imageHeight = ret.getHeight();
        targetImage.repaintImage();
        return targetImage.toStringBase64();
    }
}
