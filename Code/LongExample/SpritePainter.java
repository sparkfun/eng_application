import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Toolkit;
import java.util.Random;
import java.util.ArrayList;
import javax.swing.*;
import java.awt.geom.AffineTransform;
import java.awt.image.AffineTransformOp;
import java.awt.image.BufferedImage;
import java.io.File;
import java.awt.Font;
import javax.imageio.*;

/**
 * A class that's responsible for painting content onto a frame
 * @author Vadim Nikforov
 */
public class SpritePainter extends JPanel
{
	/**
	 * Sets a list of Sprites for the painter
	 * @param b an array of Sprites
	 */
	public void setSprites(Sprite[] b )
	{
		GameData.setSpriteList(new ArrayList<Sprite>());
		for(Sprite sp :b )
		{
				GameData.getSpriteList().add(sp);
		}
	}
	/**
	 * Public constructor
	 */
	public SpritePainter()
	{
		
	}
	
	/**
	 * Returns the size to which it should be scaled (for internal Java graphics)
	 */
	public Dimension getPreferredSize()
	{
		return new Dimension(800,480);
	}
	
	/**
	 * Internal Java method that paints content onto the panel
	 * Customized to paint sprites, effects, and extra graphics
	 * @param g the Graphics object that Java uses
	 * TODO: Make this much more readable by splitting this up into
	 * multiple private functons. As of now, it is very hard to navigate
	 * this code
	 */
	public void paint(Graphics g)
	{	
		Random rand = new Random();

		
		if(!GameData.getPaused())
		super.paintComponent(g);
		if(GameData.getGameStage()==0)
		{
			g.setColor(GameData.getTimer()%2000>1000?Color.RED:Color.CYAN);
			g.setFont(new Font("Impact", Font.PLAIN,40));
			g.drawString("MasterBlaster!",GameData.WIDTH/2-125,70);
			g.fillRect(0,0,GameData.WIDTH,20);
			g.fillRect(0,95,GameData.WIDTH,20);
			g.setColor(Color.WHITE);
			g.setFont(new Font("Impact", Font.PLAIN,30));
			g.drawString("Control Scheme:", GameData.WIDTH/2-125,160);
			g.setFont(new Font("Impact", Font.PLAIN,20));
			g.drawString("Use A and D to turn", GameData.WIDTH/2-125,200);
			g.drawString("Use W to accelerate", GameData.WIDTH/2-125,230);
			g.drawString("Use ENTER to pause", GameData.WIDTH/2-125,260);
			g.drawString("Use SPACE to shoot and begin!", GameData.WIDTH/2-125,290);
			g.setFont(new Font("Impact", Font.PLAIN,30));
			g.drawString("High Score:     " + GameData.getHighScore(), GameData.WIDTH/2-125,340);
			
			g.drawString("Difficulty:",GameData.WIDTH/2-125,GameData.HEIGHT-80);
			g.setColor(Color.RED);
			g.fillRect(GameData.WIDTH/2-125,GameData.HEIGHT-70,(int)(250*.5*GameData.getDifficulty()),20);
			g.setColor(Color.CYAN);
			g.drawRect(GameData.WIDTH/2-125,GameData.HEIGHT-70,250,20);
		}
		if(GameData.getGameStage()==1&&!GameData.getPaused())
		{
			int red = Math.min((PowerUp.getBuffTimers()[2]>0?0:GameData.getDamageAgo())*5+(100-GameData.getHealth())/2,255);
			int green = GameData.getFiredAgo()*1 + (PowerUp.getBuffTimers()[2]>0?GameData.getDamageAgo():0)*5;
			int blue = GameData.getFiredAgo()*1 + (PowerUp.getBuffTimers()[2]>0?GameData.getDamageAgo():0)*5;
			this.setBackground(new Color(red,green,blue));
			for(int i = GameData.getSpriteList().size() -1; i >=0; i--)
			{	
				int x = (int) GameData.getSpriteList().get(i).getX() - GameData.getSpriteList().get(i).getImage().getWidth(null)/2;
				int y = (int) GameData.getSpriteList().get(i).getY()- GameData.getSpriteList().get(i).getImage().getHeight(null)/2;
				BufferedImage image = GameData.getSpriteList().get(i).getImage(); 
				g.drawImage(image , x, y , null);
				if(x<image.getWidth())
				{
					g.drawImage(image , x + GameData.WIDTH, y , null);
				}
				if(x>GameData.WIDTH-image.getWidth())
				{
					g.drawImage(image , x - GameData.WIDTH, y , null);
				}
				if(y<image.getHeight())
				{
					g.drawImage(image , x, y  + GameData.HEIGHT, null);
				}
				if(y>GameData.HEIGHT-image.getHeight())
				{
					g.drawImage(image , x, y  - GameData.HEIGHT, null);
				}
				
			}
			if(PowerUp.getBuffTimers()[2]>0)
			{
				g.setColor(GameData.getTimer()%500>250?Color.CYAN:Color.WHITE);
				g.drawOval((int)(GameData.getPlayerShip().getX())-22,(int)(GameData.getPlayerShip().getY())-22,44,44);
				g.drawOval((int)(GameData.getPlayerShip().getX())-25,(int)(GameData.getPlayerShip().getY())-25,50,50);
			}
			
			for(SpecialEffect ef: GameData.getEffects())
			{
				if(ef.getType()==1)
				{
					g.setColor(GameData.getTimer()%100>50?Color.CYAN:Color.WHITE);
					g.setFont(new Font("Impact", Font.PLAIN, 20)); 
					g.drawString("+" + (int)ef.getValue(),(int)ef.getX()-8,(int)(ef.getY()-20-ef.timeLasted()/20));
				}
				if(ef.getType()==2)
				{
					g.setColor(GameData.getTimer()%100>50?Color.WHITE:(PowerUp.getBuffTimers()[2]>0?Color.CYAN:Color.RED));
					g.setFont(new Font("Impact", Font.PLAIN, 20)); 
					g.drawString("! ! ! !",(int)GameData.getPlayerShip().getX()-16,(int)(GameData.getPlayerShip().getY()-20));
				}
				if(ef.getType()==3)
				{
					g.setColor(GameData.getTimer()%100>50?Color.CYAN:Color.WHITE);
					int randX = rand.nextInt(5)-2;
					int randY = rand.nextInt(5)-2;
					g.fillOval((int)(ef.getX()-ef.timeLasted()/2.5)+randX,(int)(ef.getY()-ef.timeLasted()/2.5)+randY,(int) (ef.timeLasted()/1.25)+randX,(int) (ef.timeLasted()/1.25)+randY);
				}

			}
				g.drawImage(HUD.getMainHUDImage(),-2,0,null);
				int numberActive = 0;
				for(int i=0;i<PowerUp.NUMBER_OF_POWERUPS;i++)
				{
					if (PowerUp.getBuffTimers()[i]>0)
					{
						numberActive++;
					}
				}
				if(numberActive>0)
				{
					g.drawImage(HUD.getPowerUpHUDImage(),0,GameData.HEIGHT-40-36*numberActive,null);
					int activePosition = 0;
					for(int i=0;i<PowerUp.NUMBER_OF_POWERUPS;i++)
					{
						if(PowerUp.getBuffTimers()[i]>0)
						{
							g.drawImage(PowerUp.getBuffImages()[i],20,GameData.HEIGHT-60-36*activePosition,null);
							g.setColor(Color.RED);
							g.fillRect(60,GameData.HEIGHT-48-36*activePosition,(int) (90*PowerUp.fractionLeft(i)),8);
							g.setColor(Color.CYAN);
							g.drawRect(60,GameData.HEIGHT-48-36*activePosition,90,8);
							activePosition++;
						}
					}
				}
				g.setColor(Color.RED);
				g.fillRect(640,44,(int)(70.5*GameData.getPlayerShip().getVelocityFraction()),8);
				g.fillRect(717,44,(int)(70.5*GameData.getLaser()),8);
				g.fillRect(84,44,(int)(70.5*GameData.getHealth()/100),8);
				g.setColor(Color.CYAN);
				g.setFont(new Font("Impact", Font.PLAIN, 15)); 
				g.drawString("Speed",655,38);
				g.drawString("Weapons",724, 38);
				g.drawString("Score",25,38);
				g.drawString(""+GameData.getScore(),10,53);
				g.drawString("Health",100,38);
				g.drawRect(84,44,70,8);
				g.drawRect(640,44,70,8);
				g.drawRect(717,44,70,8);
		}
		if(GameData.getGameStage()==1&&GameData.getPaused())
		{
			g.setColor(Color.CYAN);
			g.fillRect(GameData.WIDTH/2-124,GameData.HEIGHT/2-74,248,108);
			g.setColor(Color.RED);
			g.fillRect(GameData.WIDTH/2-123,GameData.HEIGHT/2-73,246,106);
			g.setColor(Color.BLACK);
			g.fillRect(GameData.WIDTH/2-120,GameData.HEIGHT/2-70,240,100);
			g.setColor(Color.CYAN);
			g.setFont(new Font("Impact", Font.PLAIN,40));
			g.drawString("PAUSED",GameData.WIDTH/2-60,GameData.HEIGHT/2-20); 
			
		}
		if(GameData.getGameStage()==2)
		{
			g.setColor(Color.RED);
			g.setFont(new Font("Impact", Font.PLAIN,40));
			g.drawString("Game over, pal.",GameData.WIDTH/2-135,GameData.HEIGHT/2-40);
			g.setFont(new Font("Impact", Font.PLAIN,20));
			if(GameData.getTimer()%2000>800)g.drawString("Press ENTER to restart!",GameData.WIDTH/2-105,GameData.HEIGHT/2+20);
			g.drawString("Score: " + GameData.getScore(),250,GameData.HEIGHT-160);
			if(GameData.getScore() == GameData.getHighScore())
			{
				g.drawString("High Score!",250,GameData.HEIGHT-120);
			}
		}
	}
}
