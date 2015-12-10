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

public class PowerUp extends Sprite
{
	public static final int NUMBER_OF_POWERUPS = 4;
	private int type;
	private int despawnIn;
	public static int[] randomizer;
	private static int[] buffTimeLeft;
	private static BufferedImage[] powerUpImages;
	/**
	 * Public constructor
	 * @param xPos the x position of the PowerUp
	 * @param yPos the y position of the PowerUp
	 * @param ang the angle of the PowerUp
	 * @param filename the path the picture files
	 */
	public PowerUp(double xPos, double yPos, int t)
	{
		super(xPos,yPos,0,"PowerUps/PowerUp"+t);
		type = t;
		despawnIn=10*1000;
	}
	public int getType()
	{
		return type;
	}
	public void tickDespawn()
	{
		despawnIn--;
	}
	public boolean isExpired()
	{
		return despawnIn<=0;
	}
	public static double fractionLeft(int index)
	{
		return buffTimeLeft[index]/(30*1000.);
	}
	public static void tickBuffTime()
	{
		for (int i=0;i<buffTimeLeft.length;i++)
		{
			if(buffTimeLeft[i]>0)
			{
				buffTimeLeft[i]-=2;
			}	
		}
	}
	public static void setBuffTimers()
	{
		buffTimeLeft = new int[NUMBER_OF_POWERUPS];
		randomizer = new int[NUMBER_OF_POWERUPS];
		powerUpImages = new BufferedImage[NUMBER_OF_POWERUPS];
		Random rand = new Random();
		for(int i = 0;i<randomizer.length;i++)
		{
			randomizer[i]=rand.nextInt(15)+13;
			buffTimeLeft[i] = 0;
			try
			{
				powerUpImages[i]=ImageIO.read(new File("PowerUps/PowerUp"+i+".png"));
			}
			catch(Exception e){}
		}
		
	}
	public static int[] getBuffTimers()
	{
		return buffTimeLeft;
	}
	public static BufferedImage[] getBuffImages()
	{
		return powerUpImages;
	}
	public static void maximizeBuff(int num)
	{
		buffTimeLeft[num] = 30*1000;
	}
	public static void attemptToDropPowerUp(Sprite s)
	{
		for(int i = 0;i<randomizer.length;i++)
		{
			randomizer[i]--;
			if(randomizer[i]==0)
			{
				Random rand = new Random();
				GameData.getSpriteList().add(new PowerUp(s.getX(),s.getY(),i));
				randomizer[i]=rand.nextInt(15)+31;
			}
		}
	}
}
