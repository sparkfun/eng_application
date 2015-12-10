
/**
 * A class representing a generic special effect
 * @author Vadim Nikiforov
 */
public class SpecialEffect
{
	private int type;
	private int duration;
	private int limit;
	private double x,y;
	private double value;
	
	/**
	 * Public constructor
	 * @param t the type of special effect
	 * @param v a useful value for the special effect
	 */
	public SpecialEffect(int t, double v)
	{
		type=t;
		value = v;
		duration = 0;
		if(type==1||type==2)
		{
			limit = 500;
			x=GameData.getPlayerShip().getX();
			y=GameData.getPlayerShip().getY();
		}
	}
	
	/**
	 * Public constructor
	 * @param t the type of special effect
	 * @param s a Sprite to apply the affect to
	 */
	public SpecialEffect(int t, Sprite s)
	{
		type =t;
		duration = 0;
		if(type==3)
		{
			limit = 125;
			x=s.getX()-8;
			y=s.getY()-8;
		}
	}
	
	/**
	 * Returns the effect's type
	 * @return the type
	 */
	public int getType()
	{
		return type;
	}
	
	/**
	 * Returns the effect's value
	 * @return the value
	 */
	public double getValue()
	{
		return value;
	}
	
	/**
	 * Returns the effect's x coordinate
	 * @return the x coordinate
	 */
	public double getX()
	{
		return x;
	}
	
	/**
	 * Returns the effect's y coordinate
	 * @return the y coordinate
	 */
	public double getY()
	{
		return y;
	}
	
	/**
	 * Ages the effect by a unit
	 */
	public void age()
	{
		duration++;
	}
	
	/**
	 * Returns how long the effect has lasted
	 * @return the duration
	 */
	public int timeLasted()
	{
		return duration;
	}
	
	/**
	 * Returns true if the effect is expired 
	 * @return whether or not it is expired
	 */
	public boolean isExpired()
	{
		return duration>limit;
	}
}
