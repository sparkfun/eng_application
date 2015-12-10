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
 * A class that represents an asteroid
 * @author Vadim Nikiforov
 */
public class Asteroid extends Sprite
{
	public static ArrayList<BufferedImage> asteroidSprites;
	public int numberSpawned;
	private int size;
	private int id;
	
	/**
	 * Public constructor
	 * @param xPos the x position of the Asteroid
	 * @param yPos the y position of the Asteroid
	 * @param ang the angle of the Asteroid
	 * @param si the size of the Asteroid
	 * @param vel the velocity of the Asteroid
	 */
	public Asteroid(double xPos, double yPos, double ang,int si,double vel)
	{
		super(xPos, yPos, ang);
		super.setVelocity(vel);
		size=si;
		id = numberSpawned;
		numberSpawned++;
		Random rand = new Random();
		super.setImg(asteroidSprites.get(rand.nextInt(3)+3*(size-1)));
		
	}
	
	/**
	 * Tries to split the asteroid in two
	 */
	public void tryToSplit()
	{
		Random rand = new Random();
		if(size>1)
		{
			double newVel = super.getVelocity()*rand.nextDouble()+.1;
			double newAngle = rand.nextDouble()*Math.PI*2;
			GameData.getSpriteList().add(new Asteroid(super.getX(),super.getY(),newAngle,size-1,newVel));
			newVel = super.getVelocity()*rand.nextDouble()+.1;
			newAngle += (rand.nextDouble()-.5)+Math.PI;
			GameData.getSpriteList().add(new Asteroid(super.getX(),super.getY(),newAngle,size-1,newVel));
		}
	}
	
	/**
	 * Loads images for the Asteroids into memory
	 * This method is static due to how often Asteroids are created. It is more reasonable to pre-load all the sprites in memory than to load them every time a new Asteroid is formed
	 * @param filename the directory of the images
	 */
	public static void prepareSpriteSheet(String filename)
	{
		asteroidSprites=new ArrayList<BufferedImage>();
		for(int i =1;i<4;i++)
		{
			try
				{		
					asteroidSprites.add(ImageIO.read(new File(filename + "Small" + i + ".png")));
				}
				catch (IOException e) 
				{ System.out.println("something went wrong when reading a sprite file: " + filename + i + ".png"); }
		}
		for(int i =1;i<4;i++)
		{
			try
				{		
					asteroidSprites.add(ImageIO.read(new File(filename + "Medium" + i + ".png")));
				}
				catch (IOException e) 
				{ System.out.println("something went wrong when reading a sprite file: " + filename + i + ".png"); }
		}
		for(int i =1;i<4;i++)
		{
			try
				{		
					asteroidSprites.add(ImageIO.read(new File(filename + "Big" + i + ".png")));
				}
				catch (IOException e) 
				{ System.out.println("something went wrong when reading a sprite file: " + filename + i + ".png"); }
		}
	}
	
	/**
	 * Returns the ID of the Asteroid
	 * @return the ID
	 */
	public int getID()
	{
		return id;
	}
	/**
	 * Returns the size of the Asteroid
	 * @return the size
	 */
	public int getSize()
	{
		return size;
	}
	/**
	 * Jolts the Asteroid away from another Sprite
	 * @param s the other Sprite
	 * @param r the fraction of momentum retaied
	 */
	public void joltAway(Sprite s, double r)
	{
		Random rand = new Random();
		super.turn((rand.nextDouble()-.5)*Math.PI/2);
	}
	
}
