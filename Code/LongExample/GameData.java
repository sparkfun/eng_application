import java.util.ArrayList;
import java.awt.event.KeyListener;
import javax.swing.JFrame;
import java.awt.event.KeyEvent; 
import java.io.*;
import javax.swing.Timer;
import sun.audio.*;
import java.util.Random;

/**
 * A class that stores easilly accessable data for the game MasterBlaster
 * Note: having so many public variables can be a bad idea
 * TODO: implement improved data passing betwen classes
 * @author Vadim Nikiforov
 */
public class GameData
{
	public static final double MAX_SPEED = 1;
	public static final double ACCELERATION = 0.002;
	public static final double CHARGE_RATE = 0.0008;
	public static final double LASER_USAGE = 400;

	public static final int WIDTH = 800;
	public static final int HEIGHT = 480;
	
	private static Timer timerObj = null;
	private static int timer = 1;
	private static int health = 100;
	private static int score = 0;
	private static Integer hiScore = 0;
	private static double laser = 0;
	private static int damageAgo = 0;
	private static int firedAgo = 0;
	private static int gameStage = 0;
	private static boolean paused = false;
	private static int next1000 = 1000;
	private static double difficulty = 1;
	
	private static ArrayList<Sprite> spriteList;
	private static ArrayList<Collision> collisionList;
	private static Ship playerShip;
	private static JFrame bigFrame;
	private static ArrayList<SoundPlayer> soundList;
	private static ArrayList<SoundPlayer> musicList;
	private static ArrayList<SpecialEffect> effectList;
	
	private static boolean accelerating;
	
	/**
	 * changes the difficulty by a given amount
	 * @param the amount
	 */
	public static void changeDifficulty(double d)
	{
		difficulty+=d;
		difficulty=Math.min(Math.max(0,difficulty),2);
	}
	/**
	 * returns the difficulty
	 * @return the difficulty
	 */
	 public static double getDifficulty()
	 {
		 return difficulty;
	 }
	 /**
	 * returns the difficulty for use in game mechanics
	 * @return the difficulty
	 */
	 public static double getDifficultyRatio()
	 {
		 return (difficulty*.75+.25);
	 }
	/**
	 * Toggles whether or not the game is paused
	 */
	public static void togglePause()
	{
		paused = !paused;
	}
	
	/**
	 * Returns whether or not the game is paused, returning true if it is
	 * @return if the game is paused
	 */
	public static boolean getPaused()
	{
		return paused;
	}	
	
	/**
	 * Resets the game to the state when it is booted up
	 */
	public static void resetEverything()
	{
		timer=1;
		health=100;
		score=0;
		laser=0;
		damageAgo=0;
		firedAgo=0;
		gameStage=0;
		spriteList=null;
		collisionList=null;
		playerShip=null;
		accelerating=false;
		soundList = null;
		effectList = null;
		paused = false;
		next1000 = 1000;
		difficulty = 1;
	}
	
	/**
	 * Sets up the sound effects for the game
	 */
	public static void setSounds()
	{
		soundList=new ArrayList<SoundPlayer>();
		try
		{
			soundList.add(new SoundPlayer("Samples/Pew.wav"));
			soundList.add(new SoundPlayer("Samples/Boom.wav"));
			soundList.add(new SoundPlayer("Samples/Alarm1.wav"));
			
			soundList.add(new SoundPlayer("Samples/EndOfGame.wav"));
			soundList.add(new SoundPlayer("Samples/Pause.wav"));
			soundList.add(new SoundPlayer("Samples/1000Points.wav"));
			
			soundList.add(new SoundPlayer("Samples/StartOfGame.wav"));
			soundList.add(new SoundPlayer("Samples/10000Points.wav"));
			soundList.add(new SoundPlayer("Samples/PowerUp.wav"));
			System.out.println("Loaded every sound file");
		}
		catch(Exception e)
		{
			System.out.println("Unable to add SoundPlayer");
		}
	}
	/**
	 * Sets up the music for the game
	 */
	public static void setMusic()
	{
		musicList=new ArrayList<SoundPlayer>();
		try
		{
			for(int i=1;i<8;i++)
			{
				musicList.add(new SoundPlayer("Samples/Music"+i+".wav"));
				
			}
			System.out.println("Loaded every music file");
		}
		catch(Exception e)
		{
			System.out.println("Unable to add SoundPlayer for music");
		}
	}
	/**
	 * A sound from the available ArrayList
	 * @param index the index of the sound in the ArrayList
	 */
	public static void playSound(int index)
	{
			soundList.get(index).playSound();
	}
	/**
	 * Tries to play music, and plays music if there is none currently playing
	 */
	public static void playMusic()
	{
		boolean playing = false;
		for(SoundPlayer s:musicList)
		{
			playing |= !s.isDone();
		}
		if(!playing)
		{
			Random rand = new Random();
			musicList.get(rand.nextInt(7)).playSound();
			musicList.get(rand.nextInt(7)).playSound();
		}
		
	}
	public static void stopMusic()
	{
		for(int i=0;i<musicList.size();i++)
		{
			musicList.get(i).stopSound();
		}
	}
	
	/**
	 * Sets up an ArrayList of SpecialEffects
	 * @param eff an ArrayList of SpecialEffects
	 */
	public static void setEffects(ArrayList<SpecialEffect> eff){effectList=eff;}
	
	/**
	 * Returns an ArrayList of SpecialEffects
	 * @return the SpecialEffect ArrayList
	 */
	public static ArrayList<SpecialEffect> getEffects(){return effectList;}
	
	/**
	 * Creates a Timer to use for animation and calculation
	 * @param tim the Timer
	 */
	public static void setTimerObject(Timer tim){timerObj=tim;}
	/**
	 * Returns the current Timer
	 * @return the Timer
	 */
	public static Timer getTimerObject(){return timerObj;}
	
	/**
	 * Sets up the game's frame
	 * @param frame the JFrame
	 */
	public static void setBigFrame(JFrame frame){bigFrame=frame;}
	/**
	 * Returns the game's Frame
	 * @param the JFrame
	 */
	public static JFrame getBigFrame(){return bigFrame;}
	
	/**
	 * Sets up a Ship for the player
	 * @param sprite the Ship
	 */
	public static void setPlayerShip(Ship sprite){playerShip=sprite;}
	
	/**
	 * Returns the player's Ship
	 * @return the Ship
	 */
	public static Ship getPlayerShip(){return playerShip;}
	
	/**
	 * Sets the ArrayList of Sprites
	 * @param list the ArrayList to change the list of Sprites to
	 */
	public static void setSpriteList(ArrayList<Sprite> list){spriteList=list;}
	/**
	 * Returns the ArrayList of Sprites
	 * @return the ArrayList of Sprites
	 */
	public static ArrayList<Sprite> getSpriteList(){return spriteList;}
	
	/**
	 * Sets the ArrayList of Collisions
	 * @param list the ArrayList to change the list of Collisions to
	 */
	public static void setCollisionList(ArrayList<Collision> list){collisionList=list;}
	/**
	 * Returns the ArrayList of Collisions
	 * @return the ArrayList of Collisions
	 */
	public static ArrayList<Collision> getCollisionList(){return collisionList;}
	
	/**
	 * Sets whether or not the player's Ship is accelerating
	 * @param press whether or not the space bar is pressed
	 */
	public static void setAccelerating(boolean press){accelerating=press;}
	/**
	 * Returns whether or not the player's Ship is accelerating
	 * @return if the Ship is accelerating or not
	 */
	public static boolean getAccelerating(){return accelerating;}
	
	/**
	 * Increments the timer by one
	 */
	public static void incrementTimer(){timer+=1;}
	
	/**
	 * Returns the timer's value
	 * @return the timer's value
	 */
	public static int getTimer(){return timer;}
	
	/**
	 * Resets the timer's value
	 */
	public static void resetTimer(){timer=0;}
	
	/**
	 * Charges the laser by the charge rate
	 */
	public static void chargeLaser()
	{
		laser+=CHARGE_RATE;
		if(PowerUp.getBuffTimers()[3]>0)
		{
			laser+=CHARGE_RATE;
		}
		laser=Math.min(laser,1);
	}
	/**
	 * Decharges the laser by the usage times the charge rate
	 */
	public static void dechargeLaser()
	{
		if(laser>CHARGE_RATE*LASER_USAGE)
		{
			laser-=CHARGE_RATE*LASER_USAGE;
			playerShip.accelerate(-.05);
			if(PowerUp.getBuffTimers()[1]<=0)
			{
				spriteList.add(new Projectile());
			}
			if(PowerUp.getBuffTimers()[1]>0)
			{
				spriteList.add(new Projectile(true));
				spriteList.add(new Projectile(false));
			}
			if(PowerUp.getBuffTimers()[0]>0)
			{
				spriteList.add(new Projectile(Math.PI/8));
				spriteList.add(new Projectile(-Math.PI/8));
			}
			setFiredAgo(50);
			playSound(0);
		}
	}
	/**
	 * Returns how much of the laser is charged
	 * @return the charge of the laser
	 */
	public static double getLaser()
	{
		return laser;
	}
	
	/**
	 * Returns the player's health
	 * @return the player's health
	 */
	public static int getHealth(){return health;}
	/**
	 * Changes the health by a given amount
	 * @param change the change in health
	 */
	public static void changeHealth(int change){health+=change;}
	
	/**
	 * Returns the player's score
	 * @return the player's score
	 */
	public static int getScore(){return score;}
	/**
	 * Changes the score by a given amount
	 * @param change the change in score
	 */
	public static void changeScore(int change)
	{
		score+=change;
		effectList.add(new SpecialEffect(1,change));
		if(score>next1000)
		{
			if(next1000%10000==0)
			{
				playSound(7);
			}
			else
			{
				playSound(5);
			}
			next1000+=1000;
		}
	}
	
	/**
	 * Returns how long ago the ship can be damaged
	 * @return the time since the ship can be
	 */
	public static int getDamageAgo(){return damageAgo;}
	/**
	 * Sets how long until the ship can be damaged
	 * @param ago how long it will take
	 */
	public static void setDamageAgo(int ago){damageAgo=ago;}
	/**
	 * Brings the ship closer to when it can take damage
	 */
	public static void tickDamageAgo(){if(damageAgo>0)damageAgo--;}
	
	/**
	 * Returns a timer from when the laser was fired
	 * @return the timer from when the laser was fired
	 */
	public static int getFiredAgo(){return firedAgo;}
	/**
	 * Sets the timer from when the laser was fired
	 * @param ago the time to set
	 */
	public static void setFiredAgo(int ago){firedAgo=ago;}
	/**
	 * Decrements the timer for firing
	 */
	public static void tickFiredAgo(){if(firedAgo>0)firedAgo--;}
	
	/**
	 * Changes the game's stage
	 * @param s the game's stage
	 */
	public static void setGameStage(int s){gameStage=s;}
	
	/**
	 * Returns the game's stage
	 * @return the stage of the game
	 */
	public static int getGameStage(){return gameStage;}
	
	/**
	 * checks to see if the player got a high score
	 */
	public static void updateHighScore()
	{
		hiScore=Math.max(score,hiScore);
	}
	/**
	 * Returns the high score
	 * @return the high score
	 */
	public static int getHighScore()
	{
		return hiScore;
	}
	/**
	 * Reads the high score from a saved file
	 */
	public static void readHiScore()
	{
		try
		{
			InputStream file = new FileInputStream("Score/Score.dat");
			InputStream buffer = new BufferedInputStream(file);
			ObjectInput input = new ObjectInputStream (buffer);
			hiScore = (Integer) input.readObject();
			input.close();
		}
		catch(Exception e)
		{
			System.out.println("Error reading high-score file");
		}
	}
	/**
	 * Writes the current high score to a file
	 */
	public static void writeHiScore()
	{
		try 
		{
			OutputStream file = new FileOutputStream("Score/Score.dat");
			OutputStream buffer = new BufferedOutputStream(file);
			ObjectOutput output = new ObjectOutputStream(buffer);
			output.writeObject(hiScore);
			output.close();
		}
		catch(IOException ex)
		{
			System.out.println("Error writing to high score file");
		}
		
    }
	
	// handle all the keyboard input	
	public static KeyListener controls =  new KeyListener() 
	{
		public void keyPressed(KeyEvent e) 
		{
			if(e.getKeyCode()==KeyEvent.VK_W)
			{
				GameData.setAccelerating(true);
			}
		}
		public void keyReleased(KeyEvent e) 
		{ 
			if(e.getKeyCode()==KeyEvent.VK_W)
			{
				GameData.setAccelerating(false);
			}
		}
		public void keyTyped(KeyEvent e) 
		{ 
			char c = e.getKeyChar();
			if(!GameData.getPaused())
			{
				if(c =='a')
				{
					if(GameData.getGameStage()==0)
					{
						changeDifficulty(-.2);
					}
					if(GameData.getGameStage()==1)
					{
						GameData.getPlayerShip().turn(-Math.PI/4);
					}
				}
				if(c =='d')
				{
					if(GameData.getGameStage()==0)
					{
						changeDifficulty(.2);
					}
					if(GameData.getGameStage()==1)
					{
						GameData.getPlayerShip().turn(Math.PI/4);
					}
					
				}
				if(c ==' ') 
				{
					if(GameData.getGameStage()==0)
					{
						resetTimer();
						GameData.setGameStage(1);
						playSound(6);
					}
					if(GameData.getGameStage()==1)
					{
						GameData.dechargeLaser();
					}
					
				}
			}
			if(c == '\n')
			{
				if(GameData.getGameStage()==1)
				{
					GameData.togglePause();
					if(paused)
					{
						stopMusic();
					}
					playSound(4);
				}
				if(GameData.getGameStage()==2)
				{
					GameData.resetEverything();
					MasterBlaster.setUpEverything();
				}
			}
		}
	};
}
