Picoboard on a Chromebook!
==========================
Getting hardware to run from Scratch on a Chromebook has been difficult. Let us make it a little easier for you.

<img src="https://user-images.githubusercontent.com/3188387/34403572-405cef9c-eb66-11e7-8e2f-cd2b49cd81fa.jpg" alt="PB + CB TLF" width="800" />

A brief history
------------------
When the Chromebook was first released in 2011, the initial response was skeptical. Only a year later, schools had become their largest category of customer. School administrators and educators loved the low price point, ease of use, and simple connection to the world wide web for their students. The Chromebook’s place in the school system was solidified in 2014 with the introduction of [Google Classroom](https://edu.google.com/k-12-solutions/classroom/), which allowed teachers to create a central repository for student assignments, grades, and communication, not just with students, but with parents as well.

Chromebook also saw a nice boost in 2013, when the [MIT Media Lab](https://www.media.mit.edu/) released [Scratch 2.0](https://scratch.mit.edu/), their visual programming language aimed at kids between the ages of 8 and 16. Unlike its initial release, this version allowed users to create projects directly in their web browsers. It was a perfect pairing, and has given millions of kids a solid introduction to the world of coding.

Hard times for hardware
-------------------------------
The [PicoBoard](https://www.sparkfun.com/products/11888) was introduced as a way to add external sensors to the Scratch environment, bridging the gap between the virtual world and the real world. Now, instead of keystrokes or a mouse click, users could control their program with sound, or light, or a number of other sensors. Simply download the driver, add the PicoBoard extension in scratch, and you’re on your way. The problem was, when running the ChromeOS, downloading drivers wasn’t an option. ChromeOS uses extensions and apps, and unfortunately, proper ones for the PicoBoard didn’t exist. 

The ~~perfect solution~~ semi-functional workaround
------------------------------------------------------------------
There does exist a workaround that will, in fact, allow the Picoboard to work with Scratch on a Chromebook.

### First Step - installing apps and extensions
To begin, head over to the Picaxe site’s [Scratch Helper Apps page](http://www.picaxe.com/Teaching/Other-Software/Scratch-Helper-Apps), and download the S2Bot App for Linux xx-bit (zip) (depending on your system, 32- or 65-bit). Note that the link is approximately 60% of the way down the page, so don’t worry if you don’t see it right away. Follow the installation instructions, and get the S2Bot app installed on your Chromebook.

<img src="https://user-images.githubusercontent.com/3188387/34402506-2ff4afd4-eb5f-11e7-8369-44b31a5dc8b1.jpg" alt="Plugin Helper" width="600" />

Next, you’ll install the Scratch Device Plugin Helper, from the Chrome Web Store. Once again, the instructions are quite straightforward. Simply click on the + Add to Chrome button in the upper right hand corner of the Scratch Device Helper Plugin window, and Bob’s your Uncle.

### The Intermediary Steps
Here is where the juggling happens. In order for the Chromebook to fully recognize and communicate with the PicoBoard, the actual communication is going to take place through the S2Bot app. 
To start with, Plug your PicoBoard into one of the USB ports on your computer. Next, open the S2Bot app. From the dropdown menu in the upper left, choose Picoboard.
 
<img src="https://user-images.githubusercontent.com/3188387/34400540-aa7277c4-eb4f-11e7-9738-f72f59201aca.jpg" alt="S2Bot Choose Board" width="500" />

Hit the “Scan for Devices” button, and you should get ttyUSB0. While this may seem counterintuitive to Windows users, this is the port you want to select. Hit connect, and you should should be connected. 

<img src="https://user-images.githubusercontent.com/3188387/34400542-aa81c0d0-eb4f-11e7-867d-ee311c0cc2e4.jpg" alt="S2Bot Choose Port" width= "500" />


From the dropdown menu on the right, select “New Scratch Template”. Save this template, and remember where you save it. (Although the Chrome OS seems to be very good at remembering where you saved it.) You can use the default name, which is picoboard_template.sb2, or choose your own to fit your project.

Scratch Time!
-----------------

<img src="https://user-images.githubusercontent.com/3188387/34400543-aa92db86-eb4f-11e7-82a3-5b2f95f480ce.jpg" alt="Scratch Create Page" width="650" />

Open up your browser, and navigate to the [Scratch Editor](https://scratch.mit.edu/projects/editor/?tip_bar=home). Go to File>Upload from your Computer, and open the file that you just saved from the S2Bot app. Under Scripts/More Blocks, you should now see PicoBoard, with a green circle next to it, and a list of actions. This indicates that Scratch is recognizing the connection to the PicoBoard. This is good! If you don't seem to have a connection, you can check that the small circle is green, and not yellow or red. You can also pull the S2Bot app to the front, and check the lights there. You will see "PicoBoard is connected" and "Scratch is connected", and both should be showing green lights. If they aren't reconnect, then go back to Scratch, and you should be good to go!

<img src="https://user-images.githubusercontent.com/3188387/34402755-0f06ea92-eb61-11e7-8076-65d2ec3d9d9d.jpg" alt="PicoBoard Connected" width="700" />


A couple of side notes: Because the PicoBoard is running through the S2Bot app before getting to Scratch, there may be a slight lag, or you may find the response a little stilted or jerky. Also, even though, under Add Extension, Picoboard is an option, it doesn't seem to work, as there is a missing browser plugin that I don't believe exists for the ChromeOS.
