import javax.swing.*;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;
import javax.swing.Timer;

import java.util.Random;
import java.util.ArrayList;
import java.awt.*;

import java.awt.image.BufferedImage;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;

/**
 * The main class for the MasterBlaster game
 * @author Vadim Nikiforov
 */
public class MasterBlaster
{
	public static final int FRAMES = 1;
	public static SpritePainter painter;
	/**
	 * The main method
	 */
	public static void main(String[] args)  
	{
		setUpEverything();
	}
	/**
	 * Prepares the game with the proper configuration
	 */
	public static void setUpEverything()
	{
		if(GameData.getTimerObject()!=null)
		{
			GameData.getTimerObject().stop();
		}
		GameData.readHiScore();
		
		GameData.setBigFrame(new JFrame("MasterBlaster: Asteroids Beware!"));
		GameData.getBigFrame().setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		GameData.getBigFrame().setSize(GameData.WIDTH, GameData.HEIGHT);
		GameData.getBigFrame().getContentPane().setBackground(Color.BLACK);
        GameData.getBigFrame().setLocationRelativeTo(null);
        GameData.getBigFrame().setResizable(false);
        
        BufferedImage cursorImg = new BufferedImage(16, 16, BufferedImage.TYPE_INT_ARGB);
		Cursor blankCursor = Toolkit.getDefaultToolkit().createCustomCursor(
		cursorImg, new Point(0, 0), "blank cursor");
		GameData.getBigFrame().getContentPane().setCursor(blankCursor);
		
		Random rand = new Random();
	
		GameData.setSpriteList(new ArrayList<Sprite>());
		HUD.initialize();
		PowerUp.setBuffTimers();
		Asteroid.prepareSpriteSheet("Asteroid/");
		GameData.setSounds();
		GameData.setMusic();
		GameData.setPlayerShip(new Ship(GameData.WIDTH/2,GameData.HEIGHT/2,0,"Ship/Ship"));
		GameData.setCollisionList(new ArrayList<Collision>());
		GameData.setEffects(new ArrayList<SpecialEffect>());
		
		GameData.getSpriteList().add(GameData.getPlayerShip());
		painter = new SpritePainter();
		GameData.getBigFrame().getContentPane().add(painter);
		painter.setBackground(Color.BLACK);
		GameData.getBigFrame().setVisible(true);
		
		GameData.getBigFrame().setFocusable(true);
		GameData.getBigFrame().addKeyListener(GameData.controls);
				
		/* 
		 * The following code acts as a task scheduler for the game
		 * Depending on the state of the game (e.g, title screen, playing, game over)
		 * different actions need to be taken.
		 */
		ActionListener taskPerformer = new ActionListener() {
		public void actionPerformed(ActionEvent evt) 
		{
			if(GameData.getGameStage()==0)
			{
				GameData.incrementTimer();
			}
			if(GameData.getGameStage()==1&&!GameData.getPaused())
			{
				GameData.incrementTimer();
				GameData.playMusic();
				PowerUp.tickBuffTime();
				manageProjectiles();
				managePowerUps();
				manageProjectileCollisions();
				managePowerUpCollisions();
				manageShipCollisions();
				handleCollisions();
				manageEffects();
				generateAsteroids();
				handleShip();
				checkBounce();
				moveThings();
				GameData.updateHighScore();
				if(GameData.getHealth()<=0)
				{
					GameData.stopMusic();
					GameData.playSound(3);
					GameData.writeHiScore();
					GameData.setGameStage(2);
				}
			}
			if(GameData.getGameStage()==1&&GameData.getPaused())
			{
				painter.repaint();
			}
			if(GameData.getGameStage()==2)
			{
				GameData.incrementTimer();
			}
			if(GameData.getTimer()%FRAMES==0)
			{
				painter.repaint();
			}
			
		}
		};
		GameData.setTimerObject(new Timer(1, taskPerformer));
		GameData.getTimerObject().start();
	}
	private static void checkBounce()
	{
			double dx;
			double dy;
			for(Sprite sp: GameData.getSpriteList())
			{
				dx = sp.getxCenter() - (GameData.WIDTH/2);
				dy = sp.getyCenter() - (GameData.HEIGHT/2);
				
				if( (sp.getxCenter() < 0) || (sp.getxCenter() > GameData.WIDTH) || (sp.getyCenter() < 0) || (sp.getyCenter() > GameData.HEIGHT) )
				{
					if( sp.getxCenter() < 0)
					{
						sp.vectorMove(GameData.WIDTH,0);
					}
					if( sp.getxCenter() > GameData.WIDTH)
					{
						sp.vectorMove(-GameData.WIDTH,0);
					}
					if( sp.getyCenter() < 0)
					{
						sp.vectorMove(0,GameData.HEIGHT);
					}
					if( sp.getyCenter() > GameData.HEIGHT)
					{
						sp.vectorMove(0,-GameData.HEIGHT);
					}
				}
			}
	}
	private static void moveThings()
	{
		for(Sprite sp: GameData.getSpriteList())
		{
			sp.move();
		}
	}
	private static void handleShip()
	{
		GameData.tickDamageAgo();
		GameData.tickFiredAgo();
		if(GameData.getAccelerating())
		{
			GameData.getPlayerShip().accelerate(GameData.ACCELERATION);
		}
		GameData.chargeLaser();
	}
	private static void generateAsteroids()
	{
		
		if(GameData.getTimer()%((int)(3000/GameData.getDifficultyRatio()))==0)
		{	
			Random rand = new Random();
			double ang = rand.nextDouble()*Math.PI*2;
			double vel = .1+rand.nextDouble()*(0.2+(Math.sqrt(GameData.getTimer()/400000.+(GameData.getDifficultyRatio()*GameData.getTimer()/400000.))));
			int size = rand.nextInt(2)+1+((GameData.getTimer()>60*1000)?1:0);
			int image = rand.nextInt(3);
			vel/=size;
			Asteroid ast = new Asteroid(0,0,ang,size,vel);
			GameData.getSpriteList().add(ast);
		}
	}
	private static void manageProjectiles()
	{
		int i = 1;
		while(i<GameData.getSpriteList().size())
		{
			if(GameData.getSpriteList().get(i) instanceof Projectile)
			{
				if( GameData.getSpriteList().get(i).getX() < 0 || GameData.getSpriteList().get(i).getX() > GameData.WIDTH ||
					GameData.getSpriteList().get(i).getY() < 0 || GameData.getSpriteList().get(i).getY() > GameData.HEIGHT )
					{
						GameData.getSpriteList().remove(i);
					}
				else{i++;}
			}
			else{i++;}
		}
	}
	private static void managePowerUpCollisions()
	{
		for(int i = 0; i<GameData.getSpriteList().size(); i++)
		{
			if(GameData.getSpriteList().get(i) instanceof PowerUp)
			{
				for(int j = 0; j<GameData.getSpriteList().size();j++)
				{
					if(GameData.getSpriteList().get(j) instanceof Ship && Sprite.distanceBetween(GameData.getSpriteList().get(i),GameData.getSpriteList().get(j))<50) 
					{
						GameData.getCollisionList().add(new Collision(GameData.getSpriteList().get(i),i,GameData.getSpriteList().get(j),j,true));
					}
				}
			}
		}
	}
	private static void manageProjectileCollisions()
	{
		for(int i = 0; i<GameData.getSpriteList().size(); i++)
		{
			if(GameData.getSpriteList().get(i) instanceof Projectile)
			{
				for(int j = 0; j<GameData.getSpriteList().size();j++)
				{
					if(GameData.getSpriteList().get(j) instanceof Asteroid && Sprite.distanceBetween(GameData.getSpriteList().get(i),GameData.getSpriteList().get(j))<
						Math.pow(2,((Asteroid)(GameData.getSpriteList().get(j))).getSize() + 3)) 
					{
						GameData.getCollisionList().add(new Collision(GameData.getSpriteList().get(i),i,GameData.getSpriteList().get(j),j,true));
					}
				}
			}
		}
	}
	private static void manageShipCollisions()
	{
		for(int i = 0; i<GameData.getSpriteList().size(); i++)
		{
			if(GameData.getSpriteList().get(i) instanceof Ship)
			{
				for(int j = 0; j<GameData.getSpriteList().size();j++)
				{
					if(GameData.getSpriteList().get(j) instanceof Asteroid && Sprite.distanceBetween(GameData.getSpriteList().get(i),GameData.getSpriteList().get(j))<
						Math.pow(2,((Asteroid)(GameData.getSpriteList().get(j))).getSize() + 3)+8)
					{
						GameData.getCollisionList().add(new Collision(GameData.getSpriteList().get(i),i,GameData.getSpriteList().get(j),j,false));
					}
				}
			}
		}
	}
	private static void handleCollisions()
	{
		Random rand = new Random();
		for(Collision c: GameData.getCollisionList())
		{
			if(c.isDestructive())
			{
				if(c.getSprites()[0]instanceof Asteroid || c.getSprites()[1]instanceof Asteroid)
				{
					try
					{
						Sprite ast = (c.getSprites()[0] instanceof Asteroid)?c.getSprites()[0]:c.getSprites()[1];
						GameData.getEffects().add(new SpecialEffect(3,ast));
						((Asteroid)ast).tryToSplit();
						GameData.changeScore((int) (GameData.getDifficultyRatio()*(Sprite.distanceBetween(ast,GameData.getPlayerShip()) + (ast.getVelocity()*500))));
						PowerUp.attemptToDropPowerUp(ast);
						if(GameData.getSpriteList().lastIndexOf(c.getSprites()[0])>=0)
						{
							GameData.getSpriteList().remove(GameData.getSpriteList().lastIndexOf(c.getSprites()[0]));
						}
						if(GameData.getSpriteList().lastIndexOf(c.getSprites()[1])>=0)
						{
							GameData.getSpriteList().remove(GameData.getSpriteList().lastIndexOf(c.getSprites()[1]));
						}
						GameData.playSound(1);
					}
					catch(Exception e){System.out.println("Error while trying to process collisions");}
				}
				if(c.getSprites()[0]instanceof PowerUp || c.getSprites()[1]instanceof PowerUp)
				{
					if(c.getSprites()[0] instanceof PowerUp)
					{
						PowerUp.maximizeBuff(((PowerUp)c.getSprites()[0]).getType());
						GameData.getSpriteList().remove(GameData.getSpriteList().lastIndexOf(c.getSprites()[0]));
					}
					else
					{
						PowerUp.maximizeBuff(((PowerUp)c.getSprites()[1]).getType());
						GameData.getSpriteList().remove(GameData.getSpriteList().lastIndexOf(c.getSprites()[1]));
					}
					GameData.playSound(8);
				}
				
			}
			else
			{
				try
				{
					Sprite sh;
					Sprite ast;
					if(c.getSprites()[0] instanceof Ship)
					{
						sh = c.getSprites()[0];
						ast = c.getSprites()[1];
					}
					else
					{
						sh = c.getSprites()[1];
						ast = c.getSprites()[0];
					}
					if(GameData.getDamageAgo()==0)
					{
						if(PowerUp.getBuffTimers()[2]<=0)
						{
							GameData.changeHealth(-10);
						}
						else
						{
							PowerUp.getBuffTimers()[2]-=15*1000;
						}
						GameData.setDamageAgo(40);
						if(GameData.getHealth()>0)
						{
							GameData.getEffects().add(new SpecialEffect(2,0));
							GameData.playSound(1);
							GameData.playSound(2);
						}
					}
					sh.joltAway(ast,.6);
					ast.joltAway(sh,.6);
				}
				catch(Exception e){System.out.println("Error while handling sprite collisions");}
			}
		}
		GameData.setCollisionList(new ArrayList<Collision>());
	}
	private static void manageEffects()
	{
		int i = 0;
		while(i<GameData.getEffects().size())
		{
			GameData.getEffects().get(i).age();
			if(GameData.getEffects().get(i).isExpired())
			{
				GameData.getEffects().remove(i);
			}
			else
			{
				i++;
			}
		}
	}
	private static void managePowerUps()
	{
		int i = 0;
		while(i<GameData.getSpriteList().size())
		{
			if(GameData.getSpriteList().get(i)instanceof PowerUp )
			{
				((PowerUp)GameData.getSpriteList().get(i)).tickDespawn();
				if(((PowerUp) GameData.getSpriteList().get(i)).isExpired())
				{
					GameData.getSpriteList().remove(i);
				}
				else
				{
					i++;
				}
			}
			else
			{
				i++;
			}
		}
	}
	private static void nullRemover()
	{
		int i = 0;
		while(i<GameData.getSpriteList().size())
		{
			if(GameData.getSpriteList().get(i)==null)
			{
				GameData.getSpriteList().remove(i);
				System.out.println("null sprite removed");
			}
			else
			{
				i++;
			}
		}
	}
	
}

