import javax.imageio.*;
import java.io.File;
import java.io.IOException;
import java.awt.image.BufferedImage;

/**
 * A class representing a heads-up-display
 * @author Vadim Nikiforov
 */
public class HUD
{
	private static BufferedImage hudImage = null;
	private static BufferedImage powerUpImage = null;
	
	/**
	 * Sets up a HUD with a BufferedImage
	 */
	public static void initialize()
	{
			try
			{		
				hudImage = ImageIO.read(new File("HUD/MainHUD.png"));
				powerUpImage = ImageIO.read(new File("HUD/PowerUpHUD.png"));
			}
			catch (IOException e) 
			{ System.out.println("something went wrong when reading that file yo"); }
	}
	
	/**
	 * Returns the HUD's Main BufferedImage
	 * @return the BufferedImage
	 */
	public static BufferedImage getMainHUDImage()
	{
		return hudImage;
	}
	/**
	 * Returns the HUD's PowerUp BufferedImage
	 * @return the BufferedImage
	 */
	public static BufferedImage getPowerUpHUDImage()
	{
		return powerUpImage;
	}
	
}
