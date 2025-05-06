<?php

namespace Database\Seeders;

use Illuminate\Database\Console\Seeds\WithoutModelEvents;
use Illuminate\Database\Seeder;

use App\Models\AnchorLocations;

class AnchorLocationsSeeder extends Seeder
{
    public function run(): void
    {
        $data = [
            [
                'anchor' => 2,
                'location_X' => 0.0,
                'location_Y' => 0.0,
                'location_Z' => 0.0
            ],
            [
                'anchor' => 3,
                'location_X' => 500.0,
                'location_Y' => 866.0,
                'location_Z' => 0.0,
            ],[
                'anchor' => 4,
                'location_X' => 1000.0,
                'location_Y' => 0.0,
                'location_Z' => 0.0
            ],[
                'anchor' => 5,
                'location_X' => 500.0,
                'location_Y' => 288.6758,
                'location_Z' => 816.494
            ],
        ];
        AnchorLocations::insert($data);
    }
}
