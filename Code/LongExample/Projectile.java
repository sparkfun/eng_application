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
 * A class representing a projectile
 * @author Vadim Nikiforov
 */
public class Projectile extends Sprite
{
	
	private double xVel;
	private double yVel;
	
	/**
	 * Public constructor
	 */
	public Projectile()
	{
		super(GameData.getPlayerShip().getX(),GameData.getPlayerShip().getY(),GameData.getPlayerShip().getAngle(),"Projectile/Ion");
		xVel = GameData.getPlayerShip().getxVelocity();	
		yVel = GameData.getPlayerShip().getyVelocity();
		xVel+=1.9*Math.cos(super.getAngle());
		yVel+=1.9*Math.sin(super.getAngle());
	}
	/**
	 * Public constructor
	 * @param dev the angle deviation
	 */
	public Projectile(double dev)
	{
		super(GameData.getPlayerShip().getX(),GameData.getPlayerShip().getY(),GameData.getPlayerShip().getAngle()+dev,"Projectile/Ion");
		xVel = GameData.getPlayerShip().getxVelocity();	
		yVel = GameData.getPlayerShip().getyVelocity();
		xVel+=1.9*Math.cos(super.getAngle());
		yVel+=1.9*Math.sin(super.getAngle());
	}
	public Projectile(boolean whichSide)
	{
		super(GameData.getPlayerShip().getX(),GameData.getPlayerShip().getY(),GameData.getPlayerShip().getAngle(),"Projectile/Ion");
		double dx =0;
		double dy =0;
		if(whichSide)
		{
			dx=6*Math.sin(super.getAngle());
			dy=-6*Math.cos(super.getAngle());
		}
		else
		{
			dx=-6*Math.sin(super.getAngle());
			dy=6*Math.cos(super.getAngle());
		}
		super.vectorMove(dx,dy);
		xVel = GameData.getPlayerShip().getxVelocity();	
		yVel = GameData.getPlayerShip().getyVelocity();
		xVel+=1.9*Math.cos(super.getAngle());
		yVel+=1.9*Math.sin(super.getAngle());
		
	}
	/**
	 * moves the Projectile by its velocity
	 */
	public void move()
	{
		super.vectorMove(xVel,yVel);
	}
	
	
}
