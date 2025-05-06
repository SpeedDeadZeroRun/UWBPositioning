<?php

namespace Database\Seeders;

use Illuminate\Database\Console\Seeds\WithoutModelEvents;
use Illuminate\Database\Seeder;

use App\Models\Entities;

class EntitiesSeeder extends Seeder
{
    public function run(): void
    {
        $data = [
            [
                'type' => 3,
                'description' => 'Controller',
            ],
            [
                'type' => 4,
                'description' => 'Anchor A',
            ],
            [
                'type' => 4,
                'description' => 'Anchor B',
            ],
            [
                'type' => 4,
                'description' => 'Anchor C',
            ],
            [
                'type' => 4,
                'description' => 'Anchor D',
            ],
            [
                'type' => 5,
                'description' => 'Tag1',
            ],
            [
                'type' => 5,
                'description' => 'Tag2',
            ],
            [
                'type' => 5,
                'description' => 'Tag3',
            ],
            [
                'type' => 2,
                'description' => 'User1',
            ],
            [
                'type' => 2,
                'description' => 'User2',
            ],
        ];
        Entities::insert($data);
    }
}
