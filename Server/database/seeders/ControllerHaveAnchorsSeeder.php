<?php

namespace Database\Seeders;

use Illuminate\Database\Console\Seeds\WithoutModelEvents;
use Illuminate\Database\Seeder;

use App\Models\ControllerHaveAnchors;

class ControllerHaveAnchorsSeeder extends Seeder
{
    public function run(): void
    {
        $data = [
            [
                'controller' => 1,
                'have_anchor' => 2,
                'on_port' => 2,
            ],
            [
                'controller' => 1,
                'have_anchor' => 3,
                'on_port' => 3,
            ],
            [
                'controller' => 1,
                'have_anchor' => 4,
                'on_port' => 4,
            ],
            [
                'controller' => 1,
                'have_anchor' => 5,
                'on_port' => 5,
            ],
        ];
        ControllerHaveAnchors::insert($data);
    }
}
