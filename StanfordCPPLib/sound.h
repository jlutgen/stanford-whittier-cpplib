/**
 * @file sound.h
 *
 * This file exports the Sound class, which represents a sound.
 */

/*************************************************************************/
/* Stanford Portable Library                                             */
/* Copyright (c) 2014 by Eric Roberts <eroberts@cs.stanford.edu>         */
/*                                                                       */
/* This program is free software: you can redistribute it and/or modify  */
/* it under the terms of the GNU General Public License as published by  */
/* the Free Software Foundation, either version 3 of the License, or     */
/* (at your option) any later version.                                   */
/*                                                                       */
/* This program is distributed in the hope that it will be useful,       */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of        */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         */
/* GNU General Public License for more details.                          */
/*                                                                       */
/* You should have received a copy of the GNU General Public License     */
/* along with this program.  If not, see <http://www.gnu.org/licenses/>. */
/*************************************************************************/

#ifndef _sound_h
#define _sound_h


/**
 * This class encapsulates a sound file.  The sound file is specified in the
 * constructor and must be a file in either the current directory or a
 * subdirectory named <code>sounds</code>.
 *
 * As an example, the following code loads and plays the sound file
 * <code>ringtone.wav</code>:
 *
 * ~~~
 *    Sound ringtone("ringtone.wav");
 *    ringtone.play();
 * ~~~
 */
class Sound {

public:

/**
 * Creates a `%Sound` object and initializes it by reading in the contents of
 * the specified file.
 *
 * Sample usage:
 * ~~~
 * Sound sound(filename);
 * ~~~
 */
   Sound(std::string filename);


/**
 * Frees the memory associated with this `%Sound` object.
 */
   virtual ~Sound();


/**
 * Starts playing this sound.  This call returns immediately without waiting
 * for the playing to finish.
 *
 * Sample usage:
 * ~~~
 * sound.play();
 * ~~~
 */
   void play();

};

#endif
