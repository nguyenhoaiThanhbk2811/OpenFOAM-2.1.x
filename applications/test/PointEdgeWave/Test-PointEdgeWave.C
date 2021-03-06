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

Description
    Test pointEdgeWave.

\*---------------------------------------------------------------------------*/

#include "argList.H"
#include "Time.H"
#include "polyMesh.H"
#include "pointMesh.H"
#include "OSspecific.H"
#include "IFstream.H"
#include "pointEdgePoint.H"
#include "PointEdgeWave.H"

using namespace Foam;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    argList::validArgs.append("patch");

#   include "setRootCase.H"
#   include "createTime.H"
#   include "createPolyMesh.H"

    const polyBoundaryMesh& patches = mesh.boundaryMesh();

    // Get name of patch
    const word patchName = args[1];

    // Find the label in patches by name.
    label patchI = patches.findPatchID(patchName);

    {
        // Test whether any processor has patch
        label maxPatchI = patchI;

        reduce(maxPatchI, maxOp<label>());

        if (maxPatchI == -1)
        {
            FatalErrorIn(args.executable())
                << "Cannot find patch named " << patchName << exit(FatalError);
        }
    }


    // Set initial changed points to all the patch points(if patch present)
    List<pointEdgePoint> wallInfo;
    labelList wallPoints;

    if (patchI != -1)
    {
        // Retrieve the patch now we have its index in patches.
        const polyPatch& pp = mesh.boundaryMesh()[patchI];

        wallPoints = pp.meshPoints();

        wallInfo.setSize(pp.nPoints());

        forAll(pp.localPoints(), ppI)
        {
            wallInfo[ppI] = pointEdgePoint(pp.localPoints()[ppI], 0.0);
        }
    }

    // Current info on points
    List<pointEdgePoint> allPointInfo(mesh.nPoints());

    // Current info on edges
    List<pointEdgePoint> allEdgeInfo(mesh.nEdges());

    PointEdgeWave<pointEdgePoint> wallCalc
    (
        mesh,
        wallPoints,
        wallInfo,

        allPointInfo,
        allEdgeInfo,
        mesh.nPoints()  // max iterations
    );


    pointScalarField psf
    (
        IOobject
        (
            "wallDist",
            runTime.timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::AUTO_WRITE
        ),
        pointMesh::New(mesh),
        dimensionedScalar("wallDist", dimLength, 0.0)
    );

    forAll(allPointInfo, pointI)
    {
        psf[pointI] = Foam::sqrt(allPointInfo[pointI].distSqr());
    }

    Info<< "Writing wallDist pointScalarField to " << runTime.value()
        << endl;

    psf.write();

    Info<< "\nEnd\n" << endl;
    return 0;
}


// ************************************************************************* //
