/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011-2013 OpenFOAM Foundation
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

\*---------------------------------------------------------------------------*/

#include "basic2Thermo.H"
/*#include "zeroGradientFvPatchFields.H"
#include "fixedEnergyFvPatchScalarField.H"
#include "gradientEnergyFvPatchScalarField.H"
#include "mixedEnergyFvPatchScalarField.H"
#include "fixedJumpFvPatchFields.H"
#include "fixedJumpAMIFvPatchFields.H"
#include "energyJumpFvPatchScalarField.H"
#include "energyJumpAMIFvPatchScalarField.H"*/


/* * * * * * * * * * * * * * * private static data * * * * * * * * * * * * * */

namespace Foam
{
    defineTypeNameAndDebug(basic2Thermo, 0);
    defineRunTimeSelectionTable(basic2Thermo, fvMesh);
}

const Foam::word Foam::basic2Thermo::dictName("thermophysicalProperties");


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::volScalarField& Foam::basic2Thermo::lookupOrConstruct
(
    const fvMesh& mesh,
    const char* name
) const
{
    if (!mesh.objectRegistry::foundObject<volScalarField>(name))
    {
        volScalarField* fPtr
        (
            new volScalarField
            (
                IOobject
                (
                    name,
                    mesh.time().timeName(),
                    mesh,
                    IOobject::MUST_READ,
                    IOobject::AUTO_WRITE
                ),
                mesh
            )
        );

        // Transfer ownership of this object to the objectRegistry
        fPtr->store(fPtr);
    }

    return const_cast<volScalarField&>
    (
        mesh.objectRegistry::lookupObject<volScalarField>(name)
    );
}


Foam::basic2Thermo::basic2Thermo
(
    const fvMesh& mesh,
    const word& phaseName
)
:
    IOdictionary
    (
        IOobject
        (
            phasePropertyName(dictName, phaseName),
            mesh.time().constant(),
            mesh,
            IOobject::MUST_READ_IF_MODIFIED,
            IOobject::NO_WRITE
        )
    ),

    phaseName_(phaseName),

    p_(lookupOrConstruct(mesh, "p")),

    T_
    (
        IOobject
        (
            phasePropertyName("Tov"),
            mesh.time().timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh,
        dimTemperature
    )
{}


Foam::basic2Thermo::basic2Thermo
(
    const fvMesh& mesh,
    const dictionary& dict,
    const word& phaseName
)
:
    IOdictionary
    (
        IOobject
        (
            phasePropertyName(dictName, phaseName),
            mesh.time().constant(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        dict
    ),

    phaseName_(phaseName),

    p_(lookupOrConstruct(mesh, "p")),

    T_
    (
        IOobject
        (
            phasePropertyName("Tov"),
            mesh.time().timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh,
        dimTemperature
    )
{}


// * * * * * * * * * * * * * * * * Selectors * * * * * * * * * * * * * * * * //

Foam::autoPtr<Foam::basic2Thermo> Foam::basic2Thermo::New
(
    const fvMesh& mesh,
    const word& phaseName
)
{
    return New<basic2Thermo>(mesh, phaseName);
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::basic2Thermo::~basic2Thermo()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

const Foam::basic2Thermo& Foam::basic2Thermo::lookupThermo
(
    const fvPatchScalarField& pf
)
{
    /*if (pf.db().foundObject<basic2Thermo>(dictName))
    {
        return pf.db().lookupObject<basic2Thermo>(dictName);
    }
    else
    {
        HashTable<const basic2Thermo*> thermos =
            pf.db().lookupClass<basic2Thermo>();

        for
        (
            HashTable<const basic2Thermo*>::iterator iter = thermos.begin();
            iter != thermos.end();
            ++iter
        )
        {
            if
            (
                &(iter()->he().dimensionedInternalField())
              == &(pf.dimensionedInternalField())
            )
            {
                return *iter();
            }
        }
    }*/

    return pf.db().lookupObject<basic2Thermo>(dictName);
}


void Foam::basic2Thermo::validate
(
    const string& app,
    const word& a
) const
{
    if (!("e" == phasePropertyName(a)))
    {
        FatalErrorIn(app)
            << "Supported energy type is " << phasePropertyName(a)
            << ", thermodynamics package provides " << "e"
            << exit(FatalError);
    }
}

void Foam::basic2Thermo::validate
(
    const string& app,
    const word& a,
    const word& b
) const
{
    if
    (
       !(
            "e" == phasePropertyName(a)
         || "e" == phasePropertyName(b)
        )
    )
    {
        FatalErrorIn(app)
            << "Supported energy types are " << phasePropertyName(a)
            << " and " << phasePropertyName(b)
            << ", thermodynamics package provides " << "e"
            << exit(FatalError);
    }
}

void Foam::basic2Thermo::validate
(
    const string& app,
    const word& a,
    const word& b,
    const word& c
) const
{
    if
    (
       !(
            "e" == phasePropertyName(a)
         || "e" == phasePropertyName(b)
         || "e" == phasePropertyName(c)
        )
    )
    {
        FatalErrorIn(app)
            << "Supported energy types are " << phasePropertyName(a)
            << ", " << phasePropertyName(b)
            << " and " << phasePropertyName(c)
            << ", thermodynamics package provides " << "e"
            << exit(FatalError);
    }
}

void Foam::basic2Thermo::validate
(
    const string& app,
    const word& a,
    const word& b,
    const word& c,
    const word& d
) const
{
    if
    (
       !(
            "e" == phasePropertyName(a)
         || "e" == phasePropertyName(b)
         || "e" == phasePropertyName(c)
         || "e" == phasePropertyName(d)
        )
    )
    {
        FatalErrorIn(app)
            << "Supported energy types are " << phasePropertyName(a)
            << ", " << phasePropertyName(b)
            << ", " << phasePropertyName(c)
            << " and " << phasePropertyName(d)
            << ", thermodynamics package provides " << "e"
            << exit(FatalError);
    }
}


Foam::wordList Foam::basic2Thermo::splitThermoName
(
    const word& thermoName,
    const int nCmpt
)
{
    wordList cmpts(nCmpt);

    string::size_type beg=0, end=0, endb=0, endc=0;
    int i = 0;

    while
    (
        (endb = thermoName.find('<', beg)) != string::npos
     || (endc = thermoName.find(',', beg)) != string::npos
    )
    {
        if (endb == string::npos)
        {
            end = endc;
        }
        else if ((endc = thermoName.find(',', beg)) != string::npos)
        {
            end = min(endb, endc);
        }
        else
        {
            end = endb;
        }

        if (beg < end)
        {
            cmpts[i] = thermoName.substr(beg, end-beg);
            cmpts[i++].replaceAll(">","");
        }
        beg = end + 1;
    }

    if (beg < thermoName.size())
    {
        cmpts[i] = thermoName.substr(beg, string::npos);
        cmpts[i++].replaceAll(">","");
    }

    return cmpts;
}


Foam::volScalarField& Foam::basic2Thermo::p()
{
    return p_;
}


const Foam::volScalarField& Foam::basic2Thermo::p() const
{
    return p_;
}


const Foam::volScalarField& Foam::basic2Thermo::T() const
{
    return T_;
}


Foam::volScalarField& Foam::basic2Thermo::T()
{
    return T_;
}


bool Foam::basic2Thermo::read()
{
    return regIOobject::read();
}


// ************************************************************************* //
