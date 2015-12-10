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
import java.util.ArrayList;

/**
 * A class representing a sprite
 * @author Vadim Nikiforov
 */
public class Ship extends Sprite
{
	private ArrayList<BufferedImage> rotatedImgs;
	private double xVel, yVel;
	
	/**
	 * Public constructor
	 * @param xPos the x position of the Ship
	 * @param yPos the y position of the Ship
	 * @param ang the angle of the Ship
	 * @param filename the path the picture files
	 */
	public Ship(double xPos, double yPos, double ang, String filename)
	{
		super(xPos, yPos, ang, filename + "0");
		rotatedImgs = new ArrayList<BufferedImage>();
		xVel=0;
		yVel=0;
			for(int i = 0; i < 16; i++)
			{
				try
				{		
					rotatedImgs.add(ImageIO.read(new File(filename + i + ".png")));
				}
				catch (IOException e) 
				{ System.out.println("Error while reading a sprite file: " + filename + i + ".png"); }
			}		
	}
	
	/**
	 * Returns the Ship's BufferedImage
	 * @return the Ship's BufferedImage
	 */
	public BufferedImage getImage()
	{
		double angleCheck = super.getAngle()%(Math.PI*2);
		int jet = GameData.getAccelerating()?8:0;
		// TODO implement this without multiple if statements: Try using floor functions to differentiate those intervals
		if(angleCheck < Math.PI*(1/8.) || angleCheck >= Math.PI*(15/8.))
		{	super.setAngle(Math.PI*(0.));
			return rotatedImgs.get(jet+0);		}
		if(angleCheck < Math.PI*(3/8.) && angleCheck >= Math.PI*(1/8.))
		{	super.setAngle(Math.PI*(1/4.));	
			return rotatedImgs.get(jet+7);		}
		if(angleCheck < Math.PI*(5/8.) && angleCheck >= Math.PI*(3/8.))
		{	super.setAngle(Math.PI*(1/2.));	
			return rotatedImgs.get(jet+6);		}
		if(angleCheck < Math.PI*(7/8.) && angleCheck >= Math.PI*(5/8.))
		{	super.setAngle(Math.PI*(3/4.));	
			return rotatedImgs.get(jet+5);		}
		if(angleCheck < Math.PI*(9/8.) && angleCheck >= Math.PI*(7/8.))
		{	super.setAngle(Math.PI*(1.));	
			return rotatedImgs.get(jet+4);		}
		if(angleCheck < Math.PI*(11/8.) && angleCheck >= Math.PI*(9/8.))
		{	super.setAngle(Math.PI*(5/4.));	
			return rotatedImgs.get(jet+3);		}
		if(angleCheck < Math.PI*(13/8.) && angleCheck >= Math.PI*(11/8.))
		{	super.setAngle(Math.PI*(3/2.));	
			return rotatedImgs.get(jet+2);		}
		if(angleCheck < Math.PI*(15/8.) && angleCheck >= Math.PI*(11/8.))
		{	super.setAngle(Math.PI*(7/4.));	
			return rotatedImgs.get(jet+1);		}
		return super.getImage();
	}
	/**
	 * Moves the Ship
	 */
	public void move()
	{
		super.vectorMove(xVel,yVel);
	}
	/**
	 * Accelerates the Ship by the given value
	 * @param rate the rate it changes by
	 */
	public void accelerate(double rate)
	{	
		xVel+=Math.cos(super.getAngle())*rate;
		yVel+=Math.sin(super.getAngle())*rate;
		double absVel=Math.sqrt(xVel*xVel + yVel*yVel);
		if(absVel>GameData.MAX_SPEED)
		{
			double tempxVel=GameData.MAX_SPEED*(xVel/(absVel));
			double tempyVel=GameData.MAX_SPEED*(yVel/(absVel));
			xVel=tempxVel;
			yVel=tempyVel;
		}
	}
	
	/**
	 * Returns the ratio of the Ship's speed to the max speed
	 * @return the ratio
	 */
	public double getVelocityFraction()
	{
		return Math.sqrt(xVel*xVel + yVel*yVel)/GameData.MAX_SPEED;
	}
	/**
	 * Returns the Ship's velocity
	 * @return the velocity
	 */
	public double getVelocity()
	{
		return Math.sqrt(xVel*xVel + yVel*yVel);
	}
	/**
	 * Returns the Ship's x velocity
	 * @return the x velocity
	 */
	public double getxVelocity()
	{
		return xVel;
	}
	/**
	 * Returns the Ship's y velocity
	 * @return the y velocity
	 */
	public double getyVelocity()
	{
		return yVel;
	}
	/**
	 * Sets the Ship's velocity via a vector
	 * @param xCh the x-component
	 * @param yCh the y-component
	 */
	public void setVelocity(double xCh, double yCh)
	{
		xVel = xCh;
		yVel = yCh;
	}
	/**
	 * Jolts the Ship away from something
	 * @param s the Sprite to jolt away from
	 * @param r the amount of kinetic energy retained
	 */
	public void joltAway(Sprite s,double r)
	{
		Random rand = new Random();
		double dX = s.getX()-super.getX();
		double dY = s.getY()-super.getY();
		super.vectorMove((dX/Math.sqrt(dX*dX+dY*dY))*10*-r,(dY/Math.sqrt(dX*dX+dY*dY))*10*-r);
		xVel = (getVelocity()+.1)*(dX/Math.sqrt(dX*dX+dY*dY))*-r;
		yVel = (getVelocity()+.1)*(dY/Math.sqrt(dX*dX+dY*dY))*-r;
		super.turn(rand.nextDouble()*Math.PI*.5);
	}
}
