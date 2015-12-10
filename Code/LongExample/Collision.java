/**
 * A class that represents a collision between Sprites
 * @author Vadim Nikiforov
 */
public class Collision
{
	private Sprite[] sprites;
	private int[] indexes;
	private boolean destructive;
	
	/**
	 * Public constructor
	 * @param sprite1 the first Sprite
	 * @param ind1 the index of the first Sprite
	 * @param sprite2 the second Sprite
	 * @param ind2 the index of the second Sprite 
	 * @param dest whether or not the Collision is destructive
	 */
	public Collision( Sprite sprite1, int ind1, Sprite sprite2, int ind2, boolean dest )
	{
		sprites = new Sprite[2];
		indexes = new int[2];
		destructive=dest;
		if(ind1<ind2)
		{
			sprites[0]=sprite1;
			sprites[1]=sprite2;
			indexes[0]=ind1;
			indexes[1]=ind2;
		}
		else
		{
			sprites[1]=sprite1;
			sprites[0]=sprite2;
			indexes[1]=ind1;
			indexes[0]=ind2;
		}
		if(destructive) indexes[1]--;
	}
	
	/**
	 * Returns an array of the two Sprites
	 * @return the sprites
	 */
	public Sprite[] getSprites(){return sprites;}
	/**
	 * Returns an array of the indexes of the two Sprites
	 * @return the indexes
	 */
	public int[] getIndexes(){return indexes;}
	/**
	 * Returns whether or not the Collision is destructive
	 */
	public boolean isDestructive(){return destructive;}
}
