/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Class
    Foam::fileDiffusivity

Description
    Motion diffusivity read from given file name.

SourceFiles
    fileDiffusivity.C

\*---------------------------------------------------------------------------*/

#ifndef fileDiffusivity_H
#define fileDiffusivity_H

#include "motionDiffusivity.H"
#include "surfaceFields.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                           Class fileDiffusivity Declaration
\*---------------------------------------------------------------------------*/

class fileDiffusivity
:
    public motionDiffusivity
{

protected:

    // Protected data

        surfaceScalarField faceDiffusivity_;


private:

    // Private Member Functions

        //- Disallow default bitwise copy construct
        fileDiffusivity(const fileDiffusivity&);

        //- Disallow default bitwise assignment
        void operator=(const fileDiffusivity&);


public:

    //- Runtime type information
    TypeName("file");


    // Constructors

        //- Construct for the given fvMotionSolver and data Istream
        fileDiffusivity
        (
            const fvMotionSolver& mSolver,
            Istream& mdData
        );


    //- Destructor
    virtual ~fileDiffusivity();


    // Member Functions

        //- Return diffusivity field
        virtual tmp<surfaceScalarField> operator()() const
        {
            return faceDiffusivity_;
        }

        //- Do not correct the motion diffusivity
        virtual void correct()
        {}
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
