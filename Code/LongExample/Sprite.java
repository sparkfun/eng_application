import java.awt.Color;
import java.awt.Rectangle;
import java.util.Random;
import javax.imageio.*;
import java.awt.Transparency;
import java.awt.GraphicsEnvironment;
import java.awt.GraphicsConfiguration;
import java.io.File;
import java.io.IOException;
import java.awt.geom.AffineTransform;
import java.awt.image.AffineTransformOp;
import java.awt.image.BufferedImage;


/**
 * A class representing a sprite
 * @author Vadim Nikiforov
 */
public class Sprite 
{
	private double x, y;
	private double xSize, ySize;
	private double angle;
	private double velocity;
	private Color boxColor;
	private BufferedImage img;
	
	/**
	 * Public constructor
	 * @param gx the x position of the Sprite
	 * @param gys the y position of the Sprite
	 * @param gxc the x size of the Sprite
	 * @param gyc the y size of the Sprite
	 * @param ga the angle of the Sprite
	 */
	public Sprite(double gx, double gy, double gxc, double gyc, double ga)
	{
		Random rand = new Random();
		x = gx;
		y = gy;
		xSize = gxc;
		ySize = gyc;
		angle = ga;
		boxColor = new Color(rand.nextInt(256),rand.nextInt(256),rand.nextInt(256));
	}
	
	/**
	 * Public constructor
	 * @param xPos the x position of the Sprite
	 * @param yPos the y position of the Sprite
	 * @param ang the angle of the Sprite
	 * @param filename the path the picture files
	 */
	public Sprite(double xPos, double yPos, double ang, String filename)
	{
		x=xPos;
		y=yPos;
		angle=ang;
		try 
		{  img = ImageIO.read(new File(filename+".png"));
           //img = createCompatibleImage(img);
         }
         catch (IOException e) 
		{ System.out.println("Error while reading a sprite file");
		}
	}
	
	/**
	 * Public constructor
	 * @param xPos the x position of the Sprite
	 * @param yPos the y position of the Sprite
	 * @param ang the angle of the Sprite
	 */
	public Sprite(double xPos, double yPos, double ang)
	{
		x=xPos;
		y=yPos;
		angle=ang;
	}
	
	/**
	 * Sets the Sprite's image
	 * @param the image
	 */
	public void setImg(BufferedImage image)
	{
		img=image;
	}
	
	private BufferedImage createCompatibleImage(BufferedImage image)
	{
		GraphicsConfiguration gc = GraphicsEnvironment.
			getLocalGraphicsEnvironment().
			getDefaultScreenDevice().
			getDefaultConfiguration();

		BufferedImage newImage = gc.createCompatibleImage(
			image.getWidth(), 
			image.getHeight(), 
			Transparency.TRANSLUCENT);
    return newImage;
	}
	
	private BufferedImage rotateBufferedImage(double r)
	{
		AffineTransform tx = new AffineTransform();
		tx.rotate(r, img.getWidth() / 2, img.getHeight() / 2);

		AffineTransformOp op = new AffineTransformOp(tx,
        AffineTransformOp.TYPE_NEAREST_NEIGHBOR);
		return op.filter(img, null);
	}
	
	/**
	 * Returns the sprite's image
	 * @return the image
	 */
	public BufferedImage getImage(){return img;}
	
	/**
	 * Sets up the sprite
	 * @param gx the x position of the Sprite
	 * @param gys the y position of the Sprite
	 * @param gxc the x size of the Sprite
	 * @param gyc the y size of the Sprite
	 */
	public void setSprite(double gx, double gy, double gxc, double gyc)
	{
		x = gx;
		y = gy;
		xSize = gxc;
		ySize = gyc;
	}
	
	/**
	 * Accelerates up to a certain speed
	 */
	public void accelerateTo(double rate, double cap, boolean forward)
	{
		velocity+=rate;
		velocity = forward?Math.min(velocity,cap):Math.max(velocity,cap);
	}
	
	/**
	 * Moves the Sprite by a given distance
	 * @param r the distance
	 */
	public void move(double r)
	{	x += r*Math.cos(angle);
		y += r*Math.sin(angle);	}
	
	/**
	 * Moves the Sprite by a velocity unit
	 */
	public void move()
	{	x += velocity*Math.cos(angle);
		y += velocity*Math.sin(angle);	}
	
	/**
	 * Moves the Sprite via vectors
	 * @param dx the x vector
	 * @param dy the y vector
	 */
	public void vectorMove(double dx, double dy) 
	{	x += dx;	y += dy;} 
	
	/**
	 * Turns the Sprite by a given angle
	 * @param r the angle to turn by
	 */
	public void turn(double r)
	{	
		angle += Math.PI*2;
		angle += r;	
		angle %=360;
		//~ img = rotateBufferedImage(r);
		}
	
	/**
	 * Sets the Sprite's velocity
	 * @param v the velocity
	 */
	public void setVelocity(double v)
	{	velocity = v; }
	
	/**
	 * Returns the Sprite's velocity
	 * @return the velocity
	 */
	public double getVelocity()
	{	return velocity; }
	
	/**
	 * Sets the Sprite's angle
	 * @param r the angle
	 */
	public void setAngle(double r)
	{	angle = r;	}
	
	/**
	 * Sets the Sprite's angle given vectors
	 * @param dx the x vector
	 * @param dy the y vector
	 */
	public void setVectorAngle(double dy, double dx)
	{	angle = Math.atan(dy/dx);	}

	/**
	 * Returns the Sprite's x coordinate
	 * @return the x coordinate
	 */
	public double getX()
	{	return x;	}
	
	/**
	 * Returns the Sprite's y coordinate
	 * @return the y coordinate
	 */
	public double getY()
	{	return y;	}
	
	/**
	 * Returns the Sprite's x size
	 * @return the x size
	 */
	public double getxSize()
	{	return xSize;	}
	
	/**
	 * Returns the Sprite's y size
	 * @return the y size
	 */
	public double getySize()
	{	return ySize;	}
	
	/**
	 * Returns the Sprite's angle
	 * @return the angle
	 */
	public double getAngle()
	{	return angle;}
	
	/**
	 * Returns the Sprite's color
	 * @return the color
	 */
	public Color getColor()
	{	return boxColor;	}
	
	/**
	 * Returns the Sprite's visual center x coordinate
	 * @return the Sprite's visual center x coordinate
	 */
	public double getxCenter()
	{	return x+(xSize/2);	}
	
	/**
	 * Returns the Sprite's visual center y coordinate
	 * @return the Sprite's visual center y coordinate
	 */
	public double getyCenter()
	{	return y+(ySize/2);}
	
	/**
	 * Returns the distance between two Sprites
	 * This takes into account that the game is topologically toroidal
	 * @param first the first Sprite
	 * @param second the second sprite
	 * @return the distance between the two
	 */
	public static double distanceBetween(Sprite first, Sprite second)
	{
		double dx = first.getX() - second.getX();
		double dy = first.getY() - second.getY();
		double d1 =  Math.sqrt(dx*dx+dy*dy);
		
		dx = first.getX() - second.getX() -GameData.WIDTH;
		dy = first.getY() - second.getY();
		double d2 =  Math.sqrt(dx*dx+dy*dy);
		
		dx = first.getX() - second.getX();
		dy = first.getY() - second.getY() -GameData.HEIGHT;
		double d3 =  Math.sqrt(dx*dx+dy*dy);
		
		dx = first.getX() - second.getX() -GameData.WIDTH;
		dy = first.getY() - second.getY() -GameData.HEIGHT;
		double d4 =  Math.sqrt(dx*dx+dy*dy);
		
		return Math.min(Math.min(d1,d2),Math.min(d3,d4));
		
	}
	/**
	 * Empty method for polymorphism
	 * @param s the Sprite to jolt away from
	 * @param r a scalar factor
	 */
	public void joltAway(Sprite s, double r)
	{
		
	}
}
