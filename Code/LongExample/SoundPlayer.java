import java.io.File;
import java.io.IOException;

import javax.sound.sampled.*;
/**
 * A class that holds a sound file in memory for playback
 * @author Vadim Nikiforov
 */
public class SoundPlayer 
{
	private Clip clip;
	private AudioInputStream audioIn;
	private String filename;
   
	/**
	 * Public constructor
	 * @param pathName the directory of a sound file
	 */
    public SoundPlayer(String pathName)
    {
		filename=pathName;
		try
		{
			audioIn = AudioSystem.getAudioInputStream(SoundPlayer.class.getResource(pathName));
			clip = AudioSystem.getClip();
			clip.open(audioIn);
		}
		catch(Exception e){}
		
		
	}
	/**
	 * Stops any currently playing sound and starts from the beginning
	 */
    public void playSound()
    {
		clip.setMicrosecondPosition(0);
		try{
			Thread.sleep(1);
		}
		catch(Exception e){}
		clip.start();
	}
	public void stopSound()
	{
		clip.stop();
		clip.setMicrosecondPosition(0);
	}
	public boolean isDone()
	{
		return clip.getMicrosecondPosition() >= clip.getMicrosecondLength() || clip.getMicrosecondPosition()==0;
	}
}
